from torchvision import models
import torch
import torch.nn as nn
import torch.nn.init as init


def model_A(num_classes):
    model_resnet = models.resnet18() 
    num_features = model_resnet.fc.in_features
    model_resnet.fc = nn.Linear(num_features, num_classes)
    print(model_resnet)
    return model_resnet


def model_B(num_classes):
    return my_model(num_classes=num_classes, cardinality=32, depth=20)

class SEBlock(nn.Module):
    def __init__(self, channel, reduction=4):
        super(SEBlock, self).__init__()
        # Squeeze step: Global Average Pooling
        self.avg_pool = nn.AdaptiveAvgPool2d(1)
        
        # Excitation step: Fully connected layers
        self.fc1 = nn.Linear(channel, channel // reduction, bias=False)
        self.relu = nn.ReLU(inplace=True)
        self.fc2 = nn.Linear(channel // reduction, channel, bias=False)
        self.sigmoid = nn.Sigmoid()

        # Weight initialization
        init.kaiming_normal_(self.fc1.weight, mode='fan_out', nonlinearity='relu')  # Kaiming normal for fc1
        init.kaiming_normal_(self.fc2.weight, mode='fan_out', nonlinearity='relu')  # Kaiming normal for fc2
        if self.fc1.bias is not None:
            init.zeros_(self.fc1.bias)
        if self.fc2.bias is not None:
            init.zeros_(self.fc2.bias)

    def forward(self, x):
        # Squeeze operation: Global Average Pooling
        b, c, _, _ = x.size()
        y = self.avg_pool(x).view(b, c)
        
        # Excitation operation
        y = self.fc1(y)
        y = self.relu(y)
        y = self.fc2(y)
        y = self.sigmoid(y).view(b, c, 1, 1)
        
        # Apply the excitation weights to the input
        return x * y

    

class my_model_basic_block(nn.Module):
    def __init__(self, in_channels, out_channels, cardinality=32, stride=1, downsample=None, reduction=4):
        super(my_model_basic_block, self).__init__()
        width = out_channels // 4

        # 1x1 Convolution (Reduction)
        self.conv1 = nn.Conv2d(in_channels, width * cardinality, kernel_size=1, bias=False)
        self.bn1 = nn.BatchNorm2d(width * cardinality)

        # 3x3 Grouped Convolution
        self.conv2 = nn.Conv2d(width * cardinality, width * cardinality, kernel_size=3, stride=stride, padding=1, groups=cardinality, bias=False)
        self.bn2 = nn.BatchNorm2d(width * cardinality)

        # 1x1 Convolution (Expansion)
        self.conv3 = nn.Conv2d(width * cardinality, out_channels, kernel_size=1, bias=False)
        self.bn3 = nn.BatchNorm2d(out_channels)

        # Squeeze-and-Excitation Block
        self.se_block = SEBlock(out_channels, reduction)

        self.relu = nn.ReLU(inplace=True)
        self.downsample = downsample

    def forward(self, x):
        identity = x
        out = self.relu(self.bn1(self.conv1(x)))
        out = self.relu(self.bn2(self.conv2(out)))
        out = self.bn3(self.conv3(out))

        # Apply SE block after convolution
        out = self.se_block(out)

        if self.downsample is not None:
            identity = self.downsample(x)

        out += identity
        out = self.relu(out)
        return out


class my_model(nn.Module):
    def __init__(self, num_classes, cardinality=32, depth=20):
        super(my_model, self).__init__()
        
        layers_per_block = {20: [2, 2, 2], 50: [3, 4, 6, 3]}
        assert depth in layers_per_block, "Supported depths are 50 and 20"
        blocks = layers_per_block[depth]
        
        self.conv1 = nn.Conv2d(3, 64, kernel_size=7, stride=2, padding=3, bias=False)
        self.bn1 = nn.BatchNorm2d(64)
        self.relu = nn.ReLU(inplace=True)
        self.maxpool = nn.MaxPool2d(kernel_size=3, stride=2, padding=1)

        # ResNeXt Blocks
        self.layer1 = self._make_layer(64, 128, blocks[0], cardinality, stride=1)
        self.layer2 = self._make_layer(128, 256, blocks[1], cardinality, stride=2)
        self.layer3 = self._make_layer(256, 512, blocks[2], cardinality, stride=2)
        if len(layers_per_block) > 3:
            self.layer4 = self._make_layer(512, 1024, blocks[3], cardinality, stride=2)
        self.avgpool = nn.AdaptiveAvgPool2d(1)
        if len(layers_per_block) > 3:
            self.fc = nn.Linear(1024, num_classes)
        else:
            self.fc = nn.Linear(512, num_classes)
        self.init_weights()

    def _make_layer(self, in_channels, out_channels, blocks, cardinality, stride):
        downsample = None
        if stride != 1 or in_channels != out_channels:
            downsample = nn.Sequential(
                nn.Conv2d(in_channels, out_channels, kernel_size=1, stride=stride, bias=False),
                nn.BatchNorm2d(out_channels),
            )

        layers = [my_model_basic_block(in_channels, out_channels, cardinality, stride, downsample)]
        for _ in range(1, blocks):
            layers.append(my_model_basic_block(out_channels, out_channels, cardinality))

        return nn.Sequential(*layers)

    def forward(self, x):
        x = self.relu(self.bn1(self.conv1(x)))
        x = self.maxpool(x)
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        if hasattr(self, 'layer4'):
            x = self.layer4(x)
        x = self.avgpool(x)
        x = x.view(x.size(0), -1)
        x = self.fc(x)
        return x

    def init_weights(self):
        # Initialize the convolutional layers
        for m in self.modules():
            if isinstance(m, nn.Conv2d):
                init.kaiming_normal_(m.weight, mode='fan_out', nonlinearity='relu')
                if m.bias is not None:
                    init.zeros_(m.bias)

        # Initialize the fully connected layers (fc)
        if isinstance(self.fc, nn.Linear):
            init.kaiming_normal_(self.fc.weight, mode='fan_out', nonlinearity='relu')  # He initialization
            if self.fc.bias is not None:
                init.zeros_(self.fc.bias)