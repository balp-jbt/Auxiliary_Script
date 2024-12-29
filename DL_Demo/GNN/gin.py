import torch
import torch.nn.functional as F
from torch_geometric.data import DataLoader
from torch_geometric.datasets import QM9
from torch_geometric.nn import GINConv, global_mean_pool
import torch.optim as optim
import matplotlib.pyplot as plt
import os
import numpy as np

path = './data/QM9'
dataset = QM9(path)
DIPOLE_INDEX = 0  # 偶极矩在 y 中的位置

train_dataset = dataset[:10000]
val_dataset = dataset[10000:11000]
test_dataset = dataset[11000:12000]

train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)
val_loader = DataLoader(val_dataset, batch_size=32, shuffle=False)
test_loader = DataLoader(test_dataset, batch_size=32, shuffle=False)

class GIN(torch.nn.Module):
    def __init__(self, hidden_dim=64):
        super(GIN, self).__init__()
        self.conv1 = GINConv(torch.nn.Sequential(
            torch.nn.Linear(dataset.num_features + 3, hidden_dim),
            torch.nn.BatchNorm1d(hidden_dim),
            torch.nn.ReLU(),
            torch.nn.Linear(hidden_dim, hidden_dim),
            torch.nn.ReLU()
        ))
        self.conv2 = GINConv(torch.nn.Sequential(
            torch.nn.Linear(hidden_dim, hidden_dim),
            torch.nn.BatchNorm1d(hidden_dim),
            torch.nn.ReLU(),
            torch.nn.Linear(hidden_dim, hidden_dim),
            torch.nn.ReLU()
        ))
        self.conv3 = GINConv(torch.nn.Sequential(
            torch.nn.Linear(hidden_dim, hidden_dim),
            torch.nn.BatchNorm1d(hidden_dim),
            torch.nn.ReLU(),
            torch.nn.Linear(hidden_dim, hidden_dim),
            torch.nn.ReLU()
        ))
        self.lin1 = torch.nn.Linear(hidden_dim * 3, hidden_dim)
        self.lin2 = torch.nn.Linear(hidden_dim, 1)  # 输出偶极矩

    def forward(self, data):
        x = torch.cat([data.x, data.pos], dim=1)
        edge_index = data.edge_index
        batch = data.batch
        
        x1 = self.conv1(x, edge_index)
        x2 = self.conv2(x1, edge_index)
        x3 = self.conv3(x2, edge_index)
        
        x1 = global_mean_pool(x1, batch)
        x2 = global_mean_pool(x2, batch)
        x3 = global_mean_pool(x3, batch)
        
        x = torch.cat((x1, x2, x3), dim=1)
        x = F.relu(self.lin1(x))
        x = self.lin2(x)

        return x
    
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
model = GIN(hidden_dim=64).to(device)
optimizer = optim.Adam(model.parameters(), lr=0.001)

def train():
    model.train()
    total_loss = 0
    for data in train_loader:
        data = data.to(device)
        optimizer.zero_grad()
        out = model(data)
        loss = F.mse_loss(out, data.y[:, DIPOLE_INDEX].unsqueeze(1))
        loss.backward()
        optimizer.step()
        total_loss += loss.item() * data.num_graphs
    return total_loss / len(train_loader.dataset)

def evaluate(loader, need_r=False):
    model.eval()
    all_predictions = []
    all_targets = []
    total_loss = 0
    with torch.no_grad():
        for data in loader:
            data = data.to(device)
            out = model(data)
            loss = F.mse_loss(out, data.y[:, DIPOLE_INDEX].unsqueeze(1))
            total_loss += loss.item() * data.num_graphs
            if need_r:
                all_predictions.append(out.cpu().numpy())
                all_targets.append(data.y[:, DIPOLE_INDEX].cpu().numpy())
    r2_score = 0
    if need_r:
        all_predictions = np.concatenate(all_predictions, axis=0)
        all_predictions = all_predictions.squeeze()
        print(all_predictions.shape)
        all_targets = np.concatenate(all_targets, axis=0)
        print(all_targets.shape)
        mean_target = np.mean(all_targets)
        ss_total = np.sum((all_targets - mean_target) ** 2)
        ss_residual = np.sum((all_targets - all_predictions) ** 2)
        r2_score = 1 - (ss_residual / ss_total)

    return total_loss / len(loader.dataset), r2_score

# 训练模型
train_losses, val_losses = [], []
for epoch in range(1, 51):  
    train_loss = train()
    val_loss = evaluate(val_loader)[0]
    train_losses.append(train_loss)
    val_losses.append(val_loss)
    print(f'Epoch: {epoch:03d}, Train Loss: {train_loss:.4f}, Val Loss: {val_loss:.4f}')

# 测试集评估
(test_loss, R2) = evaluate(test_loader, True)
print(f'Test Loss: {test_loss:.4f}')
print(f'R2 Score: {R2:.4f}')

def dump_train_record(train_losses, valid_losses, test_loss, path="./record/gin.log"):
    folder_name = path.split('/')[1]
    os.makedirs(folder_name, exist_ok=True)
    f = open(path, 'w')
    for unit in train_losses:
        f.write(str(unit) + " ")
    f.write("\n")

    for unit in valid_losses:
        f.write(str(unit) + " ")
    f.write("\n")

    f.write(str(test_loss)) 
    f.write("\n")

    f.write(str(R2)) 
    f.write("\n")
    f.close()

dump_train_record(train_losses, val_losses, test_loss)