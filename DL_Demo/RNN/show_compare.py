import matplotlib.pyplot as plt

def parse_log(file_path):
    with open(file_path, 'r') as f:
        lines = f.readlines()
    train_loss = list(map(float, lines[0].strip().split()))
    vali_loss = list(map(float, lines[1].strip().split()))
    return train_loss, vali_loss

str1 = './loss_curve_1.log'
str2 = './loss_curve_2.log'

train_loss1, vali_loss1 = parse_log(str1)
train_loss2, vali_loss2 = parse_log(str2)

epochs = range(1, len(train_loss1) + 1)

plt.figure(figsize=(12, 6))

plt.subplot(1, 2, 1)
plt.plot(epochs, train_loss1, label='Built-in - Train Loss', color='blue', marker='o')
plt.plot(epochs, train_loss2, label='Custom - Train Loss', color='orange', marker='o')
plt.xlabel('Epoch')
plt.ylabel('Loss')
plt.title('Train Loss Comparison')
plt.legend()

plt.subplot(1, 2, 2)
plt.plot(epochs, vali_loss1, label='Built-in - Vali Loss', color='blue', marker='o')
plt.plot(epochs, vali_loss2, label='Custom - Vali Loss', color='orange', marker='o')
plt.xlabel('Epoch')
plt.ylabel('Loss')
plt.title('Validation Loss Comparison')
plt.legend()

plt.tight_layout()
plt.savefig("./compare.png")