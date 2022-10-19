import pandas as pd
import numpy as np
from keras.models import Sequential
from keras.layers import Dense

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
model.add(Dense(100, input_dim=look_back, activation='relu'))
model.add(Dense(80, activation='relu'))
model.add(Dense(50, activation='relu'))
model.add(Dense(30, activation='relu'))
model.add(Dense(15, activation='relu'))
model.add(Dense(3, activation='relu'))
model.add(Dense(1, activation='sigmoid'))

model.compile(loss='mean_squared_error', optimizer='adam', metrics=['accuracy'])
model.fit(trainX, trainY, epochs=150, batch_size=10, verbose=1)

# The code for lookback data generation was modified and taken from [ https://towardsdatascience.com/one-step-predictions-with-lstm-forecasting-hotel-revenues-c9ef0d3ef2df ].
