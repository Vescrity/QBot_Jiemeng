from PIL import Image, ImageDraw, ImageFont
import argparse
import os
current_dir = os.getcwd()
def insert_newlines(string, every=80):
    lines = string.split('\n')
    new_lines = []
    for line in lines:
        line_len = len(line)
        for i in range(0, line_len, every):
            new_lines.append(line[i:i+every])
    return '\n'.join(new_lines)
# 创建解析器对象
parser = argparse.ArgumentParser(description='Convert text file to image')
# 添加参数
parser.add_argument('file', type=str, help='the text file to be converted')
parser.add_argument('output', type=str, help='the output file name')

# 解析参数
args = parser.parse_args()

# 读取文本内容
with open(args.file, 'r', encoding='utf-8', newline='\r\n') as f:
    text = f.read().replace('\r','')
text = insert_newlines(text)
# 设置字体
font = ImageFont.truetype('./txt2img/SarasaMonoSC-Regular.ttf', 20) 

# 计算图片的大小和行高
margin = 20
line_height = int(1.8 * (font.getbbox('A')[3] - font.getbbox('A')[1]))
max_line_width = 0
num_lines = 0
for line in text.split('\n'):
    num_lines += 1
    line_width = font.getbbox(line)[2] - font.getbbox(line)[0]
    max_line_width = max(max_line_width, line_width)
img_width = max_line_width + margin * 2
img_height = line_height * num_lines + margin * 2

# 创建画布
canvas = Image.new('RGB', (img_width, img_height), color='black')

# 创建画笔
draw = ImageDraw.Draw(canvas)

# 写入文本
x = margin
y = margin
for line in text.split('\n'):
    draw.text((x, y), line, fill='white', font=font)
    y += line_height

# 保存图片
canvas.save(args.output)
print('[CQ:image,file=file:///{}/{}]'.format(current_dir, args.output), end='')
