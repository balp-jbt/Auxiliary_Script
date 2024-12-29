import torch
import torch.optim as optim
import torch.nn as nn
import torch.nn.functional as F
from torch.utils.data import Dataset, DataLoader, random_split
from model_simp import CustomModel
from dataset_simple import MyDataset
import csv
from tqdm import tqdm  # 导入 tqdm 用于进度条

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

d_in = 16
n_head = 8
ffn = 64
n_layer = 3
seq_len = 34  # 34 types of cards
d_action = 167  # 167 = 34[card] + 63[chi] + 34[peng] + 34[gang] + 1[pass] + 1[hu]

batch_size = 128
num_epochs = 200
learning_rate = 1e-5

# [I/O API]
dataset = MyDataset()

train_size = int(0.8 * len(dataset))
valid_size = int(0.1 * len(dataset))
test_size = len(dataset) - train_size - valid_size

train_dataset, valid_dataset, test_dataset = random_split(dataset, [train_size, valid_size, test_size])
train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)
valid_loader = DataLoader(valid_dataset, batch_size=batch_size, shuffle=False)
test_loader = DataLoader(test_dataset, batch_size=batch_size, shuffle=False)

model = CustomModel(d_in, ffn, n_head, n_layer, seq_len, d_action).to(device)

optimizer = optim.Adam(model.parameters(), lr=learning_rate)
criterion = nn.CrossEntropyLoss()

def train():
    with open('./training_results.csv', mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['Epoch', 'Train Loss', 'Validation Loss'])

    best_val_loss = float('inf')
    train_avg_loss, vali_loss = [], []
    for epoch in range(num_epochs):
        total_train_loss = 0
        total_train_samples = 0

        # 使用 tqdm 包裹训练数据加载器以显示进度条
        train_loader_tqdm = tqdm(train_loader, desc=f"Epoch {epoch+1}/{num_epochs}", leave=True)
        for inputs, labels in train_loader_tqdm:
            inputs, labels = inputs.to(device), labels.to(device)

            optimizer.zero_grad()

            outputs = model(inputs)

            loss = criterion(outputs, labels)
            total_train_loss += loss.item() * inputs.size(0)
            total_train_samples += inputs.size(0)

            loss.backward()
            optimizer.step()

            # 更新进度条的显示信息
            train_loader_tqdm.set_postfix({"Train Loss": loss.item()})

        avg_train_loss = total_train_loss / total_train_samples
        train_avg_loss.append(avg_train_loss)

        val_loss = evaluate()
        vali_loss.append(val_loss)

        print(f"Epoch {epoch+1}/{num_epochs}, Train Loss: {avg_train_loss:.4f}, Validation Loss: {val_loss:.4f}")

        with open('./training_results.csv', mode='a', newline='') as file:
            writer = csv.writer(file)
            writer.writerow([epoch + 1, avg_train_loss, val_loss])

        if val_loss < best_val_loss:
            best_val_loss = val_loss
            torch.save(model.state_dict(), 'best_model.pth')

    return train_avg_loss, vali_loss

def evaluate():
    model.eval()
    total_val_loss = 0
    total_val_samples = 0

    criterion = nn.CrossEntropyLoss()
    with torch.no_grad():
        for inputs, labels in valid_loader:
            inputs, labels = inputs.to(device), labels.to(device)

            outputs = model(inputs)
            loss = criterion(outputs, labels)

            total_val_loss += loss.item() * inputs.size(0)
            total_val_samples += inputs.size(0)

    avg_val_loss = total_val_loss / total_val_samples
    return avg_val_loss

def test():
    model.eval()
    total_test_loss = 0
    total_test_samples = 0

    criterion = nn.CrossEntropyLoss()
    correct_predictions = 0
    total_predictions = 0

    with torch.no_grad():
        for inputs, labels in test_loader:
            inputs, labels = inputs.to(device), labels.to(device)

            outputs = model(inputs)
            loss = criterion(outputs, labels)

            total_test_loss += loss.item() * inputs.size(0)
            total_test_samples += inputs.size(0)

            _, predicted = torch.max(outputs, 1)
            correct_predictions += (predicted == labels).sum().item()
            total_predictions += labels.size(0)

    avg_test_loss = total_test_loss / total_test_samples
    accuracy = correct_predictions / total_predictions * 100

    print(f"Test Loss: {avg_test_loss:.4f}, Test Accuracy: {accuracy:.2f}%")
    return avg_test_loss, accuracy

if __name__ == "__main__":
    train()
