import numpy as np


# divide each feature vector by its sum
def normalize_features(features):
    features = features.astype(float)
    for i in range(len(features)):
        features[i] = features[i] / features[i].sum()
    return features
