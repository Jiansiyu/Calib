"""
Installation file for Spectocalib
author: Siyu Jian
"""
from setuptools import  setup
import os


def read_requirements(fname= "requirements.txt"):
    if os.path.isfile(fname):
        with open(fname) as f:
            req = f.read().splitlines()

        return  req

setup(
    name='Calib',
    version='0.0.1',
    description='Jeffersion Lab Spectrometer Calibration Package',
    author='Siyu Jian',
    author_email='jiansiyu@gmail.com',
    url='https://python3-cookbook.readthedocs.io/zh_CN/latest/c09/p01_put_wrapper_around_function.html',
    packages=['SpectroCalib'],
    install_requires=read_requirements()
)

