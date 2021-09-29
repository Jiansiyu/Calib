"""
Preprocess pipeline for Jefferson Lab High Resolution Spectrometer Calibration(R)

Free distribution under MIT license

copyright @ Siyu Jian (sj9va@virginia.edu)
"""

import pandas as pd
from sklearn.base import BaseEstimator, TransformerMixin
from sklearn.preprocessing import PolynomialFeatures
from pandas import DataFrame
import numpy as np

class preprocess(object):
    pass


class PolynomialFeatureTransformer(BaseEstimator,TransformerMixin):
    """

    """
    def __init__(self,columns=None,max_order=5,verbose=0):
        """

        :param columns:
        :param max_order:
        :param verbose:
        """
        self.columns = columns
        self.max_order = max_order
        self.verbose = verbose

    def fit(self,X,y=None):
        """

        :param X:
        :param y:
        :return:
        """
        return self

    def transform(self,X):
        """
        :param X:
        :return:
        """
        # TODO need to check the format of the X, and adapt it to the format
        if self.columns:
            if not all([x in X.columns for x in self.columns]):
                raise TypeError('the feature in the dataset does not match the columns input!!, please check the \'columns\'')
        else:
            self.columns = X.columns

        selected = X[self.columns]
        un_selected = X.drop(columns = self.columns,axis =  1)

        poly = PolynomialFeatures(self.max_order)
        transValue = poly.fit_transform(selected)
        transHeader= poly.get_feature_names(selected.columns)

        #format dataset and concast it into one
        transformed = pd.DataFrame(transValue,columns=transHeader)

        # if un_selected.empty():
        transformed = pd.concat([transformed,un_selected],axis=1)
        return  transformed

    def fit_transform(self, X, y=None, **fit_params):
        """

        :param X:
        :param y:
        :param fit_params:
        :return:
        """
        return self.fit(X,y).transform(X)