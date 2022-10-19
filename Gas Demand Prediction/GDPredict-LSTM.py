import pandas as pd
import numpy as np
from keras.layers import LSTM
from keras.models import Sequential
from keras.layers import Dense
from keras.layers import Dropout

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

train_years = [2013, 2014, 2015, 2016, 2017]
test_years = [2018]

train = total[total['year'].isin(train_years)]
test = total[total['year'].isin(test_years)]

look_back = 24 * 7
trainX = []
trainY = []
for i in range(look_back, len(train)):
    trainX.append(train[i-look_back:i, 1])
    trainY.append(train[i, 0])
trainX, trainY = np.array(trainX), np.array(trainY)

trainX = np.reshape(trainX, (trainX.shape[0], trainX.shape[1], 1))

testX = []
testY = []
for i in range(look_back, len(test)):
    testX.append(test[i-look_back:i, 1])
    testY.append(test[i, 0])
testX, testY = np.array(testX), np.array(testY)

testX = np.reshape(testX, (testX.shape[0], testX.shape[1], 1))
testX, testY = np.array(testX), np.array(testY)

model = Sequential()
model.add(LSTM(units = 200, return_sequences = True, input_shape = (trainX.shape[1], 1)))
model.add(Dropout(0.5))
model.add(LSTM(units = 200, return_sequences = True))
model.add(Dropout(0.5))
model.add(LSTM(units = 50, return_sequences = True))
model.add(Dropout(0.5))
model.add(LSTM(units = 50))
model.add(Dropout(0.5))
model.add(Dense(units = 30))
model.add(Dense(units = 10))
model.add(Dense(units = 1))

model.compile(optimizer = 'adam', loss = 'mean_squared_error')
model.fit(trainX, trainY, epochs = 2, batch_size = 32, verbose=2)

# The code for lookback data generation was modified and taken from [ https://towardsdatascience.com/one-step-predictions-with-lstm-forecasting-hotel-revenues-c9ef0d3ef2df ].
