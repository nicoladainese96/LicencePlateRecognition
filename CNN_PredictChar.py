import ConvolutionalNN as myCNN
import torch
import string
import skimage
import numpy as np

def predict(image_path):
    """
    Image_path can be local (if found in the same folder of the script) or complete
    Image must be of a number 0-9 or a capital letter A-Z.
    Only resolution accepted: 32x32.
    """

    CNN = myCNN.Net().double()
    state_dict = torch.load('saved_model.pth')
    CNN.load_state_dict(state_dict)

    #IncompatibleKeys(missing_keys=[], unexpected_keys=[]) means "no error"

    img = skimage.io.imread(image_path, as_gray=True)
    #skimage.io.imshow(img)
    img = np.array(img).reshape((1,1,32,32)) #vector of a single element with 1 channel of 32x32 pixels
    img = img/img.max() # in case it wasn't normalized
    img = torch.from_numpy(img)

    my_dict = define_dict()

    outputs = CNN(img)
    _, predicted = torch.max(outputs.data, 1)
    predicted = predicted.numpy()
    char = my_dict[predicted[0]]
    print(char)


def define_dict():
    capital_letters = list(string.ascii_uppercase)
    numbers = list(['%d'%i for i in range(10)])
    num_and_letters = numbers+capital_letters # list of all the numbers and characters to recognize

    numbers_and_chars_dict = {}
    for i in range(36):
        numbers_and_chars_dict[i] = num_and_letters[i]
    return numbers_and_chars_dict
