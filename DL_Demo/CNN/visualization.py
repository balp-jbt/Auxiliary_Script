import torch
import torchvision.models as models
from captum.attr import Saliency, LayerGradCam
from torchvision import transforms
from PIL import Image
import os
import numpy as np
import matplotlib.pyplot as plt

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

save_dir = "./checkpoints"
model = torch.load(os.path.join(save_dir, 'best_model.pt'))
model.to(device)
model.eval()

data = np.load('./pathmnist_64.npz')
images = data['train_images']
random_index = np.random.randint(0, len(images))
im = Image.fromarray(images[random_index].astype(np.uint8))

new_size = (256, 256)
im = im.resize(new_size)
im.save("input_image.png")

preprocess = transforms.Compose([
    transforms.Resize(256),
    transforms.ToTensor(),
    transforms.Normalize(mean=[.5], std=[.5])
])

img = Image.open("input_image.png").convert('RGB')
input_tensor = preprocess(img).unsqueeze(0).to(device)

output = model(input_tensor)
target_class = torch.argmax(output, dim=1)

def compute_saliency_map(model, input_tensor, target_class):
    saliency = Saliency(model)

    saliency_map = saliency.attribute(input_tensor, target=target_class)

    saliency_map = saliency_map.squeeze(0).cpu().detach().numpy()
    if saliency_map.ndim == 3:
        saliency_map = np.mean(saliency_map, axis=0)

    saliency_map = np.abs(saliency_map)
    saliency_map -= saliency_map.min()
    saliency_map /= saliency_map.max()
    saliency_map *= 255
    saliency_map = saliency_map.astype(np.uint8)

    return saliency_map

saliency_map = compute_saliency_map(model, input_tensor, target_class)

plt.imshow(saliency_map, cmap='hot')
plt.colorbar()
plt.title("Saliency Map")
plt.axis('off')
plt.savefig("saliency_features.png", bbox_inches='tight')
plt.close()

print("Saliency map saved as saliency_features.png")

def get_conv_features(model, input_tensor):
    features = []

    def hook_fn(module, input, output):
        features.append(output)

    hook = model.conv1.register_forward_hook(hook_fn)
    with torch.no_grad():
        model(input_tensor)
    hook.remove()

    return features
features = get_conv_features(model, input_tensor)

feature_map = features[0].squeeze(0).cpu().detach().numpy()

def plot_conv_features(feature_map, num_features=16, save_path="conv_features.png"):
    num_feature_maps = feature_map.shape[0]
    num_to_show = min(num_feature_maps, num_features)

    fig, axes = plt.subplots(4, 4, figsize=(12, 12))
    axes = axes.flatten()

    for i in range(num_to_show):
        axes[i].imshow(feature_map[i], cmap='viridis')
        axes[i].axis('off')
        axes[i].set_title(f'Feature {i+1}')

    plt.tight_layout()
    plt.savefig(save_path)
    plt.close()

plot_conv_features(feature_map, save_path="conv_features.png")

print("Convolution feature maps saved as conv_features.png")
