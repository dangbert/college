import numpy as np
from sklearn.neighbors import KNeighborsClassifier
from sklearn.svm import SVC
from sklearn.metrics import confusion_matrix


def train_classifier(features, labels, args):
    print("\tstarting training")
    if args.classifier == 'knn':
        clf = KNeighborsClassifier(n_neighbors=args.knn_k)
        clf.fit(features, labels)
        return clf
    elif args.classifier == 'svm':
        clf = SVC(C=args.svm_lambda, kernel='linear', probability=True)
        clf.fit(features, labels)
        return clf
    else:
        raise NotImplementedError()


def make_predictions(clf, features):
    prediction_probs = clf.predict_proba(features)
    return np.argmax(prediction_probs, axis=-1), \
        np.max(prediction_probs, axis=-1)

def get_confusion(labels, predictions):
    conf_mat = confusion_matrix(
        y_true=labels,
        y_pred=predictions,
        #labels=['Cats', 'Dogs'])
        labels=[0, 1])
    accuracy = float((conf_mat[0,0] + conf_mat[1,1]) / conf_mat.sum())
    return (conf_mat, accuracy)

def show_confusion(labels, predictions):
    print("\nlabels:")
    print(labels)
    print("prediction:")
    print(predictions)
    conf_mat, accuracy = get_confusion(labels, predictions)
    print(conf_mat)
    print("accuracy is: " + str(accuracy))
