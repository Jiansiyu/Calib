"""
Preprocess pipeline for Jefferson Lab High Resolution Spectrometer Calibration(R)

Free distribution under MIT license

copyright @ Siyu Jian (sj9va@virginia.edu)
"""
import pandas
from sklearn.base import BaseEstimator, TransformerMixin
from sklearn.feature_selection import SelectFromModel
from sklearn.feature_selection import SelectKBest
from sklearn.feature_selection import f_regression
from sklearn.feature_selection import mutual_info_regression

class feature_selection(BaseEstimator,TransformerMixin):
    """
    """
    def __init__(self,model,max_features=20,**params):
        """
        :param model:
        :param max_features:
        :param params:
        """
        pass
    def fit(self,X,y=None):
        pass
    def transform(self,X):
        pass

    def fit_transform(self, X, y=None, **fit_params):
        return self.fit(X,y).transform(X)
