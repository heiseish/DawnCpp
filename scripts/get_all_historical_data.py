import json
import os
import requests
import datetime
import time

epoch = datetime.datetime.utcfromtimestamp(0)

def unix_time_millis(dt):
    return (dt - epoch).total_seconds() * 1000

def unix_time():
    return int(datetime.datetime.now().strftime("%s")) * 1000 

binance_api = 'https://api.binance.com'

config_file = os.path.join(os.getcwd(), '..', 'config/trading.json')
with open(config_file, 'r') as f:
    config = json.load(f)
symbols = config[0]['symbols']

for symbol in symbols:
    symbol = symbol.replace('/', '')
    output_file = os.path.join(os.getcwd(), '..', 'data/{}.json'.format(symbol))
    if os.path.exists(output_file):
        print('{} exist. Skipping'.format(symbol))
    ts_ms = unix_time()
    ts_last = 1483243199000
    data = []
    delta = float('inf')
    while ts_last < ts_ms:
        url = 'https://api.binance.com/api/v1/klines?symbol={}&interval={}&startTime={}'.format(symbol, '5m', ts_last)
        res = requests.get(url)
        if res.status_code != 200:
            print('Error code != 200')
            break
        res = res.json()
        data += res
        ts_last = res[-1][6]
        ts_last += 1
        if abs(ts_last - ts_ms) >= delta:
            print('Something wrong. Value not getting closer')
            break
        delta = abs(ts_last - ts_ms)
        time.sleep(0.2)
    with open(output_file, 'w') as f:
        json.dump(data, f, indent=4)
    print('Done with {}'.format(symbol))
