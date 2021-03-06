import os
import sys
#import boto3
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import MaxNLocator
import pandas as pd


AWS_S3_CREDS = {"aws_access_key_id": os.getenv("AWS_ACCESS_KEY"), # os.getenv("AWS_ACCESS_KEY")
                "aws_secret_access_key": os.getenv("AWS_SECRET_KEY"),  # os.getenv("AWS_SECRET_KEY")
                }

# pod-name_alpha_beta_k_numberofrounds
#s3_client = boto3.client('s3',**AWS_S3_CREDS)


"""Upload a file to an S3 bucket

:param file_name: File to upload
:param bucket: Bucket to upload to
:param object_name: S3 object name. If not specified then file_name is used
:return: True if file was uploaded, else False
"""


## Upload the file
#file_name =  os.getenv("MY_POD_NAME") + "_" + os.getenv("ALPHA") + "_" +  os.getenv("BETA") + "_" +  os.getenv("K_SAMPLE_SIZE") + "_" + os.getenv("NUMBER_OF_ROUNDS")
#try:
#    response = s3_client.upload_file(file_name, "bucket")
#except ClientError as e:
#    print(e)
#

def get_files(how_many):
    return [f"workers_results/worker-envars-fieldref-statefulset-{i}_res.csv" for i in range(int(how_many))]


def parse_data(results_dict, f, times):
    first_row = f.readline()
    times.append(int(first_row))
    for line in f.readlines()[1:]:
        round_number, accepted_color = line.strip().split(",")
        results_dict[str(int(round_number)+1)][accepted_color] += 1


def plot(results_dict, how_many):
    #import pdb;pdb.set_trace()

    df = pd.DataFrame.from_dict(results_dict, orient="index")
    print(df.index)
    ax = df.plot(kind="bar")
    ax.set_xlabel("round_number")
    ax.set_ylabel("number of acceptors")
    plt.legend(loc='upper left')
    ax.yaxis.set_major_locator(MaxNLocator(integer=True))
    ax.set_ylim((0, how_many + 10))
    ax.set_xticklabels(ax.get_xticks(), rotation=0)

    plt.savefig('data.png')
    plt.show()
    plt.close()


def build_dict(results_dict, number_of_rounds):
    for i in range(int(number_of_rounds)):
        results_dict[str(i+1)] = {"0":0, "1":0}


def main():
    results_dict = {}
    how_many = int(sys.argv[1])
    number_of_rounds = int(sys.argv[2])
    build_dict(results_dict, number_of_rounds)
    files = get_files(how_many)
    times = []
    counter = 0
    for output_file in files:
        if os.stat(output_file).st_size == 0:
            counter += 1
            continue
        with open(output_file) as f:
            parse_data(results_dict, f, times)
    print(results_dict)
    total_dis_agree = 0
    for round in results_dict.values():
        total_dis_agree += min(round.values())
    perc = (total_dis_agree * 100) / ((how_many-counter)*number_of_rounds)
    print(f"dis agree {perc}%")
    # plot(results_dict, int(how_many))
    df = pd.DataFrame(times)
    print(f"the avg is :{df.apply(np.average)}")


if __name__ == '__main__':
    main()
