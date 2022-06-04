AWS_S3_CREDS = {"aws_access_key_id": os.getenv("AWS_ACCESS_KEY"), # os.getenv("AWS_ACCESS_KEY")
                "aws_secret_access_key": os.getenv("AWS_SECRET_KEY"),  # os.getenv("AWS_SECRET_KEY")
                }

# pod-name_alpha_beta_k_numberofrounds
s3_client = boto3.client('s3',**AWS_S3_CREDS)

REPLICAS = 10
K_SAMPLE_SIZE = 1
ALPHA = 0.5
BETA = 8
NUMBER_OF_ROUNDS = 10
POD_NAME_PREFIX = "worker-envars-fieldref-statefulset-"

for i in range(0, REPLICAS):
    file_name = POD_NAME_PREFIX + str(i) + "_" + str(ALPHA) + "_" +  str(BETA) + "_" + str(K_SAMPLE_SIZE) + "_" + str(NUMBER_OF_ROUNDS)

    s3_client.download_file('BUCKET_NAME', file_name, POD_NAME_PREFIX + str(i))
