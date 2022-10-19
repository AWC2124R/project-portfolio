import pandas as pd
import numpy as np

from lightgbm import LGBMRegressor
from xgboost import XGBRegressor
from catboost import CatBoostRegressor

from sklearn.neighbors import KNeighborsClassifier
from sklearn.ensemble import RandomForestRegressor


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

train_labels = np.array(train['공급량'])
train_features = train.drop(['공급량', '연월일', 'total_time'], axis = 1)

test_labels = np.array(test['공급량'])
test_features = test.drop(['공급량', '연월일', 'total_time'], axis = 1)



model = LGBMRegressor(n_estimators = 1000, learning_rate= 0.1, max_depth=6)
model.fit(train_features, train_labels, eval_set=[(test_features, test_labels)], early_stopping_rounds= 50,verbose = 100)

model = XGBRegressor(objective='reg:squarederror', max_depth=4)
model.fit(train_features, train_labels, verbose=2)

model = CatBoostRegressor(verbose=0, n_estimators=100)
model.fit(train_features, train_labels, verbose=2)

knn = KNeighborsClassifier(n_neighbors=1, n_jobs=-1)
knn.fit(train_features, train_labels)

rf = RandomForestRegressor(n_estimators = 100, random_state = 42, verbose=2, max_depth=16)
rf.fit(train_features, train_labels)
