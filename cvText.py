import cv2
from PIL import ImageFont, ImageDraw, Image
import numpy as np
import textwrap
import math
import numpy as np

font_type= "font/simsun.ttc"
def cpu_text_bbox(text,size):
    font = ImageFont.truetype(font_type, size)
    # ascent, descent = font.getmetrics()
    # (width, baseline), (offset_x, offset_y) = font.font.getsize(text)
    (x_min, y_min, x_max, y_max) = font.getmask(text).getbbox()
    w_text = x_max - x_min
    h_text = y_max - y_min

    return w_text,h_text

def draw_text(text,w,h,color):
    len_text = len(text)
    im = Image.new('RGB', (w, h), color) 
    draw = ImageDraw.Draw(im) 
    w_text_50,h_text_50 = cpu_text_bbox(text,50)
    
    line_num = 1
    while True:

        new_s = math.floor(1.0*h/h_text_50*50/line_num)
        w_text,h_text = cpu_text_bbox(text,new_s)
        print(new_s)
        if w_text < 1.0 * w * line_num+10:

            # new_s = 54
            font = ImageFont.truetype(font_type, new_s)
            wrap_w = math.ceil(1.0 * len_text / w_text * w)
            print(wrap_w)
            para = textwrap.wrap(text, width=wrap_w)
            for line in para: 
                w, h = draw.textsize(line, font=font) 
                draw.text((0,0), line, font=font)
    
            break

        line_num = line_num + 1
    img = cv2.cvtColor(np.asarray(im), cv2.COLOR_RGB2GRAY)
    
    return img 

        


def draw_text_by_mask(img,mask,text):
    text_mask = draw_text(text , img.shape[1], img.shape[0],(0, 0, 0, 0))

    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))

    dst = cv2.dilate(text_mask, kernel)
    hollow_mask = (text_mask == 0) & (dst != 0)
    patch_mask = hollow_mask | ((text_mask==255) & (mask!=255))
    print(patch_mask.shape,img.shape,patch_mask.sum())
    img[patch_mask] = (img[patch_mask]*0)

    return img




img = np.ones((100,200), np.uint8)*200
# img = cv2.rectangle(img, (30, 30), (150, 70), (255), 5)
font = cv2.FONT_HERSHEY_SIMPLEX
img = cv2.putText(img, 'test', (0, 100), font, 3, (255, 255, 255), 2)

img = draw_text_by_mask(img,img,'你好')
print( img.shape)
cv2.imshow( 'hh',img)
cv2.waitKey(0)
