import argparse
import os
import json
import matplotlib.pyplot as plt
import seaborn as sns
sns.set(rc={'figure.figsize':(11, 4)})

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='plit kline')
    parser.add_argument(
        '--pair_name',
        help='Pair name to plot',
        type=str,
        default='ETHBTC'
    )
    args = parser.parse_args()
    data_file = os.path.join(os.getcwd(), '..', 'data/{}.json'.format(args.pair_name))
    with open(data_file, 'r') as f:
        json_data = json.load(f)
    time = [a[0] for a in json_data[-100:]]
    open_p = [a[1] for a in json_data[-100:]]
    plt.plot(time, open_p)
    plt.show()



