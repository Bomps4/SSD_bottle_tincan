import sys
import matplotlib.pyplot as plt
import os
import pandas as pd
import re
import numpy as np
import xml.etree.ElementTree as ET
from sklearn.metrics import average_precision_score
import glob

RECTANGLE=re.compile("draw_rectangle\(\(-*[0-9]+\,-*[0-9]+\,-*[0-9]+\,-*[0-9]+\)")




