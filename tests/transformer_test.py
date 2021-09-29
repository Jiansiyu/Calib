import os
from SpectroCalib.feature_engineer._preprocess import PolynomialFeatureTransformer
from pandas import DataFrame
import pandas as pd
data =  pd.read_csv('../data/CRex/RHRS/CRex_DataSet_21743.csv')
