from PIL import Image
import os

def concatenate_images(truth_id, regressive_id, teacher_id, gap=2):
    id = truth_id // 5
    imgs_truth, imgs_teacher, imgs_regressive = [], [], []
    
    threshold = 5 if id != 19 else 4
    for i in range(threshold):
        image_truth_path = f"./output/truth_{truth_id}.png"
        image_teacher_path = f"./output/teacher_{teacher_id}.png"
        image_regressive_path = f"./output/regressive_{regressive_id}.png"
        imgs_truth.append(Image.open(image_truth_path))
        imgs_teacher.append(Image.open(image_teacher_path))
        imgs_regressive.append(Image.open(image_regressive_path))
        truth_id += 1
        teacher_id += 1
        regressive_id += 1
    
    concatenated_width = 32 * 5 + gap * 4
    concatenated_truth_image = Image.new('RGB', (concatenated_width, 32))
    for i, img in enumerate(imgs_truth):
        concatenated_truth_image.paste(img, (i * (32 + gap), 0))
    concatenated_truth_image.save(f"./predict/truth_{id}.png")

    concatenated_teacher_image = Image.new('RGB', (concatenated_width, 32))
    for i, img in enumerate(imgs_teacher):
        concatenated_teacher_image.paste(img, (i * (32 + gap), 0))
    concatenated_teacher_image.save(f"./predict/teacher_{id}.png")

    concatenated_regressive_image = Image.new('RGB', (concatenated_width, 32))
    for i, img in enumerate(imgs_regressive):
        concatenated_regressive_image.paste(img, (i * (32 + gap), 0))
    concatenated_regressive_image.save(f"./predict/regressive_{id}.png")

if __name__ == "__main__":
    os.makedirs("./predict", exist_ok=True)
    for id in range(1, 100, 5):
        concatenate_images(id, id, id)
