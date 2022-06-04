import os
import boto3

AWS_S3_CREDS = {"aws_access_key_id": os.getenv("AWS_ACCESS_KEY"), # os.getenv("AWS_ACCESS_KEY")
                "aws_secret_access_key": os.getenv("AWS_SECRET_KEY"),  # os.getenv("AWS_SECRET_KEY")
                }

# pod-name_alpha_beta_k_numberofrounds
s3_client = boto3.client('s3',**AWS_S3_CREDS)


"""Upload a file to an S3 bucket

:param file_name: File to upload
:param bucket: Bucket to upload to
:param object_name: S3 object name. If not specified then file_name is used
:return: True if file was uploaded, else False
"""


# Upload the file
file_name =  os.getenv("MY_POD_NAME") + "_" + os.getenv("ALPHA") + "_" +  os.getenv("BETA") + "_" +  os.getenv("K_SAMPLE_SIZE") + "_" + os.getenv("NUMBER_OF_ROUNDS")
try:
    response = s3_client.upload_file(file_name, "bucket")
except ClientError as e:
    print(e)

