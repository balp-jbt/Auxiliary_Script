import matplotlib.pyplot as plt

def parse_log(file_path):
    with open(file_path, 'r') as f:
        lines = f.readlines()
    train_loss = list(map(float, lines[0].strip().split()))
    vali_loss = list(map(float, lines[1].strip().split()))
    return train_loss, vali_loss

def plt_curve(path, name):
    train_loss, vali_loss = parse_log(path)
    plt.clf()
    plt.plot(range(1, len(train_loss) + 1), train_loss, label='Train Loss', 
             color='#1f77b4', marker='o', markersize=5, linestyle='-', linewidth=2)
    plt.plot(range(1, len(vali_loss) + 1), vali_loss, label='Vali Loss', 
             color='#ff7f0e', marker='o', markersize=5, linestyle='-', linewidth=2)
    plt.xlabel('Epoch', fontsize=12)
    plt.ylabel('Loss', fontsize=12)
    plt.legend(loc='upper right', fontsize=10)
    plt.xticks(fontsize=10)
    plt.yticks(fontsize=10)
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.savefig(f"./{name}-loss.png", dpi=300)
    plt.close()

names = ["gat", "gcn", "graphsage", "gin"]
for name in names:
    path = f"./record/{name}.log"
    plt_curve(path, name)