import os
import boto3

AWS_S3_CREDS = {"aws_access_key_id": os.getenv("AWS_ACCESS_KEY"), # os.getenv("AWS_ACCESS_KEY")
                "aws_secret_access_key": os.getenv("AWS_SECRET_KEY"),  # os.getenv("AWS_SECRET_KEY")
                }
s3_client = boto3.client('s3',**AWS_S3_CREDS)
print(s3_client.list_buckets())