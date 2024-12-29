from genericpath import exists
import torch
import torch.nn as nn
import torch.optim as optim
from torch.optim.lr_scheduler import CosineAnnealingLR
import models
import os
import argparse
from tqdm import tqdm
import torchvision.transforms as transforms
import medmnist
from medmnist import INFO, Evaluator
from tensorboardX import SummaryWriter

# Note that: here we provide a basic solution for training and testation.
# You can directly change it if you find something wrong or not good enough.

def run(model, train_set, valid_set, test_set, criterion, optimizer, scheduler, save_dir, data_path, num_epochs=20):

    def train(model, train_loader, optimizer, criterion):
        model.train(True)
        total_loss = 0.0

        bar = tqdm(train_loader, desc='train')
        i = 0
        for inputs, labels in bar:
            inputs = inputs.to(device)
            labels = labels.to(device)
            optimizer.zero_grad()
            outputs = model(inputs)

            labels = labels.squeeze().long()
            loss = criterion(outputs, labels)
            writer.add_scalar('Loss/train', loss.item(), epoch * len(train_loader) + i)
            _, predicted = torch.max(outputs, 1)
            correct = (predicted == labels).sum().item()
            total = labels.size(0)
            train_accuracy = correct / total
            writer.add_scalar('ACC/train', train_accuracy, epoch * len(train_loader) + i)

            loss.backward()
            optimizer.step()

            total_loss += loss.item() * inputs.size(0)
            bar.set_postfix(loss=loss.item())
            i += 1

        epoch_loss = total_loss / len(train_loader.dataset)
        train_accuracy = correct / total
        return epoch_loss

    def valid_or_test(model, valid_loader, split):
        model.train(False)

        y_true = torch.tensor([])
        y_score = torch.tensor([])
        for inputs, labels in tqdm(valid_loader, desc=split):
            inputs = inputs.to(device)
            labels = labels.to(device)
            outputs = model(inputs)
            labels = labels.squeeze().long()

            loss = criterion(outputs, labels)

            outputs = outputs.softmax(dim=-1)
            labels = labels.float().resize_(len(labels), 1)

            y_true = torch.cat((y_true, labels.cpu()), 0)
            y_score = torch.cat((y_score, outputs.cpu()), 0)
            
        scheduler.step()
        y_true = y_true.numpy()
        y_score = y_score.detach().numpy()
        evaluator = Evaluator(data_flag, split, size=64, root=data_path)
        metrics = evaluator.evaluate(y_score)

        return metrics[0], metrics[1], loss

    best_acc = 0.0
    if not exists(save_dir):
        os.makedirs(save_dir)
    
    for epoch in range(num_epochs):
        print(f'epoch:{epoch+1}/{num_epochs}')
        print('*' * 100)

        # Re-define data transformations at the start of each epoch
        train_set = transforms.Compose([
            transforms.ToTensor(),
            transforms.RandomHorizontalFlip(),
            transforms.RandomVerticalFlip(),
            transforms.RandomRotation(90),
            transforms.ColorJitter(brightness=0.3, contrast=0.3, saturation=0.3, hue=0.2),
            transforms.RandomResizedCrop(64, scale=(0.5, 1.0)),
            transforms.RandomAffine(degrees=30, translate=(0.2, 0.2), scale=(0.8, 1.2), shear=10),
            transforms.RandomGrayscale(p=0.2),
            transforms.RandomErasing(p=0.5, scale=(0.02, 0.25)),
            transforms.Normalize(mean=[.5], std=[.5])
        ])
        train_set = DataClass(root=data_path, split='train', transform=train_set, size=64, download=download)
        train_loader = torch.utils.data.DataLoader(train_set, batch_size=batch_size, shuffle=True, num_workers=10)
        train_loss = train(model, train_loader, optimizer, criterion)
        print(f"training: {train_loss:.4f}")

        valid_loader = torch.utils.data.DataLoader(valid_set, batch_size=batch_size, shuffle=False, num_workers=10)
        with torch.no_grad():
            val_auc, val_acc, loss = valid_or_test(model, valid_loader, 'val')
        print(f'valid auc: {val_auc:.3f}  acc:{val_acc:.3f}')
        writer.add_scalar('AUC/validation', val_auc, epoch)
        writer.add_scalar('ACC/validation', val_acc, epoch)
        writer.add_scalar('Loss/validation', loss, epoch)

        if val_acc > best_acc:
            best_acc = val_acc
            best_model = model

    # Test the best model
    test_loader = torch.utils.data.DataLoader(test_set, batch_size=batch_size, shuffle=False, num_workers=10)
    with torch.no_grad():
        test_auc, test_acc, loss = valid_or_test(model, test_loader, 'test')
    print(f'test auc: {test_auc:.3f}  acc:{test_acc:.3f}  loss:{loss:.3f}')
        
    torch.save(best_model, os.path.join(save_dir, f'{model_name}.pt'))



if __name__ == '__main__':
    model_name = "new-model-up-aug"
    torch.cuda.empty_cache()
    parser = argparse.ArgumentParser(description='hw1')
    parser.add_argument('--save_dir', type=str, default='./checkpoints/', help='model save path')
    args = parser.parse_args()

    # data preparation
    writer = SummaryWriter('./log-new-up-aug') 

    data_flag = 'pathmnist'
    download = False
    info = INFO[data_flag]
    task = info['task']
    n_channels = info['n_channels']
    n_classes = len(info['label'])
    DataClass = getattr(medmnist, info['python_class'])

    data_transform = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize(mean=[.5], std=[.5])
    ])

    # data_transform = transforms.Compose([
    #     transforms.ToTensor(),
    #     transforms.Normalize(mean=[.5], std=[.5])
    # ])

    # download dataset first and modify the data_path accordingly
    data_path = './'
    train_dataset = DataClass(root=data_path, split='train', transform=data_transform, size=64, download=download)
    valid_dataset = DataClass(root=data_path, split='val', transform=data_transform, size=64, download=download)
    test_dataset = DataClass(root=data_path, split='test', transform=data_transform, size=64, download=download)

    # train_dataset = DataClass(root=data_path, split='train', transform=data_transform, size=64, download=download)
    # valid_dataset = DataClass(root=data_path, split='val', transform=data_transform, size=64, download=download)
    # test_dataset = DataClass(root=data_path, split='test', transform=data_transform, size=64, download=download)

    # about training
    num_epochs = 50
    # num_epochs = 20
    lr = 0.001
    batch_size = 64

    # model initialization
    # model = models.model_A(num_classes=n_classes)
    model = models.model_B(num_classes=n_classes)

    device = torch.device("cuda:1" if torch.cuda.is_available() else "cpu")
    model = model.to(device)

    # define loss function and optimizer
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters(), lr=lr)
    scheduler = CosineAnnealingLR(optimizer, T_max=num_epochs)

    run(model, train_dataset, valid_dataset, test_dataset, criterion, optimizer, scheduler, args.save_dir, data_path, num_epochs=num_epochs)

    writer.close()