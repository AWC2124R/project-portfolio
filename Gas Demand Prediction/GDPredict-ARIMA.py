from tkinter import N
import pandas as pd
import numpy as np
from statsmodels.tsa.arima.model import ARIMA

total = pd.read_csv('data.csv', encoding='cp949')
total['연월일'] = pd.to_datetime(total['연월일'])

total['year'] = total['연월일'].dt.year
total['month'] = total['연월일'].dt.month
total['day'] = total['연월일'].dt.day
total['weekday'] = total['연월일'].dt.weekday

total["total_time"] = pd.to_datetime(total['연월일']).astype(np.int64)

total["total_time"] = total["total_time"] / 86400000000000
total["total_time"] = (total["total_time"] - total["total_time"][0] + total["시간"] / 24) * 100

d_map = {}
for i, d in enumerate(total['구분'].unique()):
    d_map[d] = i

total['구분'] = total['구분'].map(d_map)
total = pd.get_dummies(total, columns=["구분"])

X = total["공급량"]
X = X.values
size = int(len(X) * 1)
train, test = X[0:size], X[size:len(X)]`
history = [x for x in train]

predictions = []
for t in range(N):
    model = ARIMA(history, order=(5, 1, 0))
    model_fit = model.fit()
    output = model_fit.forecast()

    yhat = output[0]
    obs = yhat

    predictions.append(yhat)
    history.append(obs)
 
# Rolling forecast ARIMA model modified and taken from [ https://machinelearningmastery.com/arima-for-time-series-forecasting-with-python/ ].
