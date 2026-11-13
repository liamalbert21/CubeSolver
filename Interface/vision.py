import numpy as np
import cv2

from math import sqrt
from enum import Enum
from dataclasses import dataclass
from sklearn.cluster import KMeans
from collections import Counter

# TODO
# - Get mean color data
# - Set thresholds and convert extract colors
# - Feed colors (relatively positioned) through algorithm to acquire solve
#   instructions

WINDOW_NAME = "Cube Color Extractor"
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600

FOREGROUND_COLOR_BGR = (0, 0, 255)
SHAPE_THICKNESS = 3
SIDE_LENGTH = SCREEN_WIDTH / 2.5

KEY_ESC = 27
KEY_ENTER = 13

# From experiment
TARGET_RED = (178, 97, 118)
TARGET_ORANGE = (213, 139, 104)
TARGET_YELLOW = (190, 186, 92)
TARGET_GREEN = (81, 215, 55)
TARGET_BLUE = (28, 106, 218)
TARGET_WHITE = (152, 155, 199)

@dataclass
class Box:
    x: int
    y: int
    width: int
    height: int


class Color(Enum):
    RED = 0
    ORANGE = 1
    YELLOW = 2
    GREEN = 3
    BLUE = 4
    WHITE = 5


thresh = {
    Color.RED: [],
    Color.ORANGE: [],
    Color.YELLOW: [],
    Color.GREEN: [],
    Color.BLUE: [],
    Color.WHITE: []
}


'''
def extract_face_colors(frame, box):
    cell_dim = box.width // 9
    clsx = ""
    while not clsx:
        clsx = input("Enter colors separated by whitespace: ").split()

    for y in range(3):
        for x in range(3):
            start = (
                box.x + cell_dim * (1 + 3 * x),
                box.y + cell_dim * (1 + 3 * y)
            )
            crop = frame[
                int(start[1]):int(start[1] + cell_dim),
                int(start[0]):int(start[0] + cell_dim)
            ]
            crop = cv2.cvtColor(crop, cv2.COLOR_BGR2HSV)

            lower_blue_hsv = []
            upper_blue_hsv = []
'''

def extract_face_colors(frame, box):
    assert box.width == box.height

    num_colors = 9
    loop = int(sqrt(num_colors))
    cell_dim = box.width // num_colors

    _colors = []
    classifications = ""
    while not classifications:
        classifications = input("Enter colors separated by whitespace\n").split()

    for y in range(loop):
        for x in range(loop):
            start = (
                box.x + cell_dim * (1 + num_colors / loop * x),
                box.y + cell_dim * (1 + num_colors / loop * y)
            )
            crop = frame[
                int(start[1]):int(start[1] + cell_dim),
                int(start[0]):int(start[0] + cell_dim)
            ]
            crop = cv2.cvtColor(crop, cv2.COLOR_BGR2RGB)

            # Flatten the crop to a 1D array of pixels
            crop_flattened = crop.reshape(-1, 3)

            cluster = KMeans(n_clusters=1)
            count_metadata = Counter(cluster.fit_predict(crop_flattened))
            estimate = cluster.cluster_centers_[count_metadata.most_common(1)[0][0]]
            _colors.append(estimate)

            if __debug__:
                print(estimate)

    return _colors, classifications


source = cv2.VideoCapture(0)
if not source.isOpened():
    print("ERROR: Failed to open camera app!")
    exit()

cv2.namedWindow(WINDOW_NAME, cv2.WINDOW_NORMAL)
cv2.resizeWindow(WINDOW_NAME, SCREEN_WIDTH, SCREEN_HEIGHT)

colors = [[], [], [], [], [], []]

while True:
    ret, frame = source.read()
    if not ret:
        break

    h, w, *_ = frame.shape
    box = Box(
        w // 2 - SIDE_LENGTH // 2,
        h // 2 - SIDE_LENGTH // 2,
        SIDE_LENGTH, SIDE_LENGTH
    )
    cv2.rectangle(
        frame, np.array((box.x, box.y), np.uint16), 
        np.array((box.x + box.width, box.y + box.height),
            np.uint16),
        FOREGROUND_COLOR_BGR, SHAPE_THICKNESS
    )

    key = cv2.waitKey(1)
    if key == KEY_ENTER:
        _colors, classifications = extract_face_colors(frame, box)

        for i in range(9):
            c = _colors[i]
            x = classifications[i]

            match x:
                case 'r': colors[0].append(c)
                case 'o': colors[1].append(c)
                case 'y': colors[2].append(c)
                case 'g': colors[3].append(c)
                case 'b': colors[4].append(c)
                case 'w': colors[5].append(c)

    elif key == KEY_ESC:
        break

    cv2.imshow(WINDOW_NAME, frame)

for i in range(6):
    avg = [0, 0, 0]
    for j in range(9):
        avg[0] += colors[i][j][0]
        avg[1] += colors[i][j][1]
        avg[2] += colors[i][j][2]

    avg[0] /= 9
    avg[1] /= 9
    avg[2] /= 9
    print(avg)

source.release()
cv2.destroyWindow(WINDOW_NAME)
