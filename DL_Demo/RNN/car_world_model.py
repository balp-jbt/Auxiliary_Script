import torch
import torch.nn as nn
import torch.optim as optim
import pickle
import matplotlib.pyplot as plt
from PIL import Image
import os
import numpy as np

use_builtin = True
ori_evaluate = False
fig_name = "loss_curve_1" if use_builtin else "loss_curve_2"
evaluate_mode = "teacher" if ori_evaluate else "autoregressive"

with open("car_racing_data_32x32_120.pkl", "rb") as f:
    data = pickle.load(f)

class WorldModel(nn.Module):
    def __init__(self, action_size, hidden_size, output_size):
        super(WorldModel, self).__init__()

        self.cnn = nn.Sequential(
            nn.Conv2d(3, 16, kernel_size=3, stride=2, padding=1),  # [3, 32, 32] -> [16, 16, 16]
            nn.ReLU(),
            nn.Conv2d(16, 32, kernel_size=3, stride=2, padding=1),  # [16, 16, 16] -> [32, 8, 8]
            nn.ReLU(),
            nn.Flatten()
        )

        self.cnn_output_size = 32 * 8 * 8

        self.lstm = nn.LSTM(self.cnn_output_size + action_size, hidden_size, batch_first=True)

        self.fc = nn.Linear(hidden_size, output_size)

        self.W_x = nn.Linear(self.cnn_output_size + action_size, 4 * hidden_size, bias=False)
        self.W_h = nn.Linear(hidden_size, 4 * hidden_size, bias=False)
        self.b = nn.Parameter(torch.zeros(hidden_size * 4))

    def lstm_cell(self, x, hidden):
        h_prev, c_prev = hidden
        h_prev = hidden[0].squeeze(0)  #[1, batch_size, hidden_size] ->  [batch_size, hidden_size]
        c_prev = hidden[1].squeeze(0)

        a = self.W_x(x) + self.W_h(h_prev) + self.b
        i_t = torch.sigmoid(a[:, :hidden_size])
        f_t = torch.sigmoid(a[:, hidden_size:2*hidden_size])
        o_t = torch.sigmoid(a[:, 2*hidden_size:3*hidden_size])
        g_t = torch.tanh(a[:, 3*hidden_size:])

        c_t = f_t * c_prev + i_t * g_t
        h_t = o_t * torch.tanh(c_t)

        return h_t, c_t

    def forward(self, state, action, hidden=None, use_builtin=use_builtin):
        if use_builtin:
            return self.forward_builtin(state, action, hidden)
        return self.forward_custom(state, action, hidden)

    def forward_custom(self, state, action, hidden=None):
        features = self.cnn(state)  # [batch_size, cnn_output_size]
        lstm_input = torch.cat([features, action], dim=1)  # [batch_size, cnn_output_size + action_size]

        if hidden is None:
            h_t = torch.zeros(lstm_input.size(0), self.hidden_size).to(lstm_input.device)
            c_t = torch.zeros(lstm_input.size(0), self.hidden_size).to(lstm_input.device)
            hidden = (h_t, c_t)

        h_t, c_t = self.lstm_cell(lstm_input, hidden)

        next_state_pred = self.fc(h_t)
        return next_state_pred, (h_t, c_t)

    def forward_builtin(self, state, action, hidden=None):
        """
        TODO: Implement your model here. You should implement two models, one using built-in lstm layers and one implemented by yourself.

        Forward pass for the WorldModel.
        Args:
            state: Tensor of shape [batch_size, 3, 32, 32] (current RGB image at this time step).
            action: Tensor of shape [batch_size, action_size] (3-dimensional action vector).
            hidden: Tuple of hidden states for LSTM, (h_t, c_t), each of shape [1, batch_size, hidden_size].
            
        Returns:
            next_state_pred: Tensor of shape [batch_size, output_size] (flattened next state prediction, 3*32*32).
            hidden: Updated hidden state tuple (h_t, c_t) for the LSTM.
        """
        features = self.cnn(state)  # [batch_size, cnn_output_size]

        lstm_input = torch.cat([features, action], dim=1).unsqueeze(1)  # [batch_size, 1, cnn_output_size + action_size]

        lstm_output, hidden = self.lstm(lstm_input, hidden)  # lstm_output: [batch_size, 1, hidden_size]

        next_state_pred = self.fc(lstm_output.squeeze(1))
        return next_state_pred, hidden

class WorldModelDataLoader:
    def __init__(self, data, batch_size, sequence_length, device):
        self.data = data
        self.batch_size = batch_size
        self.sequence_length = sequence_length
        self.device = device

        # 拆分数据为 train, valid, test 集合
        split_train = int(0.8 * len(self.data))
        split_valid = int(0.1 * len(self.data))
        self.train_data = self.data[:split_train]
        self.valid_data = self.data[split_train:split_train + split_valid]
        self.test_data = self.data[split_train + split_valid:]

        self.set_train()

    def set_train(self):
        self.current_data = self.train_data
        self.index = 0
        self.sub_index = 0  # 子序列的起始索引

    def set_valid(self):
        self.current_data = self.valid_data
        self.index = 0
        self.sub_index = 0

    def set_test(self):
        self.current_data = self.test_data
        self.index = 0
        self.sub_index = 0

    def get_batch(self):
        states, actions = [], []
        batch_data = self.current_data[self.index: self.index + self.batch_size]

        for sequence in batch_data:
            state_seq = [torch.tensor(step[0]) for step in sequence[self.sub_index:self.sub_index + self.sequence_length]]
            action_seq = [torch.tensor(step[1]) for step in sequence[self.sub_index:self.sub_index + self.sequence_length]]
            if len(state_seq) < self.sequence_length:
                pad_len = self.sequence_length - len(state_seq)
                state_seq += [torch.zeros_like(state_seq[0])] * pad_len
                action_seq += [torch.zeros_like(action_seq[0])] * pad_len

            states.append(torch.stack(state_seq))
            actions.append(torch.stack(action_seq))

        self.sub_index += self.sequence_length
        if self.sub_index >= len(self.current_data[self.index]):
            self.index += self.batch_size  
            self.sub_index = 0  
        states = torch.stack(states).to(self.device)
        actions = torch.stack(actions).to(self.device)

        end_flag = self.index >= len(self.current_data)

        return states, actions, end_flag


device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
action_size = 3
hidden_size = 128
output_size = 32 * 32 * 3
batch_size = 16
sequence_length = 100
num_epochs = 50
learning_rate = 3e-4


data_loader = WorldModelDataLoader(data, batch_size, sequence_length, device)
model = WorldModel(action_size=action_size, hidden_size=hidden_size, output_size=output_size).to(device)
criterion = nn.MSELoss()
optimizer = optim.Adam(model.parameters(), lr=learning_rate)

def train(num_epochs=50):
    best_val_loss = float('inf') 

    train_avg_loss, vali_loss = [], []

    for epoch in range(num_epochs):
        model.train()
        data_loader.set_train()
        total_train_loss = 0
        total_train_samples = 0

        while True:
            states, actions, end_flag = data_loader.get_batch()
            batch_size_actual = states.size(0)
            # Initialize the hidden state (h_0, c_0) for the LSTM, resetting it for each new batch
            hidden = (torch.zeros(1, batch_size_actual, hidden_size).to(device),
                      torch.zeros(1, batch_size_actual, hidden_size).to(device))
            # Loop through each time step in the sequence
            for t in range(sequence_length - 1):
                current_state = states[:, t]
                action = actions[:, t]
                next_state = states[:, t + 1].view(batch_size_actual, -1)

                next_state_pred, hidden = model(current_state, action, hidden)
                loss = criterion(next_state_pred, next_state)

                optimizer.zero_grad()
                loss.backward()
                optimizer.step()

                hidden = tuple([h.detach() for h in hidden])

                total_train_loss += loss.item()
                total_train_samples += 1

            if end_flag:
                break

        avg_train_loss = total_train_loss / total_train_samples
        val_loss = evaluate()

        train_avg_loss.append(avg_train_loss)
        vali_loss.append(val_loss)

        print(f"Epoch {epoch + 1}/{num_epochs}, Train Loss: {avg_train_loss:.4f}, Val Loss: {val_loss:.4f}")


        if val_loss < best_val_loss:
            best_val_loss = val_loss
            torch.save(model.state_dict(), "world_model_best.pth")
            print("Best model saved.")

    plt.rcParams['font.family'] = 'Times New Roman'
    plt.figure(figsize=(8, 6))
    plt.plot(range(1, num_epochs + 1), train_avg_loss, label="Train Loss", marker='o')
    plt.plot(range(1, num_epochs + 1), vali_loss, label="Validation Loss", marker='x')
    plt.xlabel("Epoch")
    plt.ylabel("Loss")
    plt.title("Training and Validation Loss Over Epochs")
    plt.legend()
    plt.grid()
    plt.savefig(f"{fig_name}.png")
    plt.show()

    f = open(f"{fig_name}.log", 'w')
    for unit in train_avg_loss:
        f.write(str(unit) + " ")
    f.write("\n")

    for unit in vali_loss:
        f.write(str(unit) + " ")
    f.write("\n")
    f.close()

def evaluate():
    model.eval()
    data_loader.set_valid()
    total_val_loss = 0
    total_val_samples = 0

    with torch.no_grad():
        while True:
            states, actions, end_flag = data_loader.get_batch()
            batch_size_actual = states.size(0)

            hidden = (torch.zeros(1, batch_size_actual, hidden_size).to(device),
                      torch.zeros(1, batch_size_actual, hidden_size).to(device))

            for t in range(sequence_length - 1):
                current_state = states[:, t]
                action = actions[:, t]
                next_state = states[:, t + 1].view(batch_size_actual, -1)

                next_state_pred, hidden = model(current_state, action, hidden)
                loss = criterion(next_state_pred, next_state)

                total_val_loss += loss.item()
                total_val_samples += 1

            if end_flag:
                break

    avg_val_loss = total_val_loss / total_val_samples
    return avg_val_loss

def predict_treacher():
    model.eval()
    data_loader.set_test()

    with torch.no_grad():
        states, actions, end_flag = data_loader.get_batch()
        datapoint_state, datapoint_action = states[0].unsqueeze(0), actions[0].unsqueeze(0)

        hidden = (torch.zeros(1, 1, hidden_size).to(device),
                    torch.zeros(1, 1, hidden_size).to(device))
        
        predictions, ground_truths = [], []

        for t in range(sequence_length - 1):
            current_state = datapoint_state[:, t]

            action = datapoint_action[:, t]
            next_state = datapoint_state[:, t + 1].view(1, -1)
            next_state_pred, hidden = model(current_state, action, hidden)
                    
            predictions.append(next_state_pred.cpu().numpy().reshape(3, 32, 32))
            ground_truths.append(next_state.cpu().numpy().reshape(3, 32, 32))
    
    return predictions, ground_truths

            
def predict_autoregressive():
    model.eval()
    data_loader.set_test()

    with torch.no_grad():
        states, actions, end_flag = data_loader.get_batch()
        datapoint_state, datapoint_action = states[0].unsqueeze(0), actions[0].unsqueeze(0)
        hidden = (torch.zeros(1, 1, hidden_size).to(device),
                  torch.zeros(1, 1, hidden_size).to(device))
        
        predictions = []

        current_state = datapoint_state[:, 0]
        action = datapoint_action[:, 0]

        for t in range(sequence_length - 1):
            next_state_pred, hidden = model(current_state, action, hidden)
            predictions.append(next_state_pred.cpu().numpy().reshape(3, 32, 32))

            current_state = next_state_pred.reshape(1, 3, 32, 32)
            action = datapoint_action[:, t + 1]

    return predictions

def save_imgs(data, type, output_dir="output", filename="comparison.png"):
    os.makedirs(output_dir, exist_ok=True)
    for i, (unit) in enumerate(data):
        image = np.clip((unit * 255), 0, 255).astype(np.uint8)
        image = np.transpose(image, (1, 2, 0))
        path = os.path.join(output_dir, f"{type}_{i+1}.png")
        Image.fromarray(image).save(path)

def test():
    model.eval()
    data_loader.set_test()
    total_test_loss = 0
    total_test_samples = 0

    with torch.no_grad():
        while True:
            states, actions, end_flag = data_loader.get_batch()
            batch_size_actual = states.size(0)

            hidden = (torch.zeros(1, batch_size_actual, hidden_size).to(device),
                      torch.zeros(1, batch_size_actual, hidden_size).to(device))

            for t in range(sequence_length - 1):
                current_state = states[:, t]
                action = actions[:, t]
                next_state = states[:, t + 1].view(batch_size_actual, -1)
                next_state_pred, hidden = model(current_state, action, hidden)
                loss = criterion(next_state_pred, next_state)

                total_test_loss += loss.item()
                total_test_samples += 1

            if end_flag:
                break

    avg_test_loss = total_test_loss / total_test_samples
    print(f"Test Loss: {avg_test_loss:.4f}")

train(num_epochs=50)
evaluate()
test()
predictions, ground_truths = predict_treacher()
save_imgs(ground_truths, "truth")
save_imgs(predictions, "teacher")
predictions = predict_autoregressive()
save_imgs(predictions, "regressive")