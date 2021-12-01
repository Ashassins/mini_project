from typing import List, Tuple
from PIL import Image

def px2u16(px: List[int]) -> int:
    r = (px[1] / 255) * ((1 << 6) - 1)
    g = (px[2] / 255) * ((1 << 7) - 1)
    b = (px[3] / 255) * ((1 << 6) - 1)
    rv = int(r)
    rv = rv << 6 | int(g)
    rv = rv << 5 | int(b)
    return rv


def load_img(path: str) -> List[int]:
    im = Image.open(path)
    px = im.load()
    img = []
    for i in range(im.size[0]):
        for j in range(im.size[1]):
            img.append(px2u16(px[i, j]))
    return img

def dump_img_arr(path: str) -> Tuple[str, str]:
    img = load_img(path)
    name = path.split("/")[-1].split(".")[0].lower()
    c_arr = ""
    c_arr += f"const uint16_t {name}[{len(img)}] = {{\n\t"
    for i, px in enumerate(img):
        c_arr += f"{hex(px)}, "
        if i % 5 == 4:
            c_arr += "\n\t"

    c_arr += "\n};"
    return c_arr, f"extern const uint16_t {name}[{len(img)}];"

if __name__ == "__main__":
    from sys import argv
    hdr = """#ifndef INVADERS_H
#define INVADERS_H
#include <stdint.h>"""
    c_file = '#include "invaders.h"\n'
    for path in argv[1:]:
        c_data, hdr_data = dump_img_arr(path)
        hdr += "\n" + hdr_data
        c_file += "\n" + c_data

    hdr += "\n#endif"
    with open("invaders_dat.h", "w") as f:
        f.write(hdr)

    with open("invaders_dat.c", "w") as f:
        f.write(c_file)
