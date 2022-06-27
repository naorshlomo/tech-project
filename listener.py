#!/usr/bin/env python
import time

from kubernetes import client, config
import os
import sys

def main():
    print("starting the syncronizer")
    sys.stdout.flush()


    # it works only if this script is run by K8s as a POD
    config.load_incluster_config()
    # use this outside pods
    # config.load_kube_config()

    replicas = os.environ.get('REPLICAS', None)

    v1 = client.CoreV1Api()

    ret = v1.list_pod_for_all_namespaces(watch=False )
    counter =0
    while counter != int(replicas) + 20:
        ret = v1.list_pod_for_all_namespaces(watch=False)
        counter = 0
        for j in ret.items:
            if (j.status.phase== "Running"):
                counter+=1
                # print("%s\t%s\t%s" %
                #       (j.status.pod_ip, j.metadata.namespace, j.metadata.name))
        time.sleep(2)
        

    print("starting the avalanche process")
    sys.stdout.flush()
    #change to numberofrounds + sleep + ....
    os.system("timeout 10000 ./avalanche")
    sys.stdout.flush()
    print("finished running the avalanche process")
    sys.stdout.flush()

if __name__ == '__main__':
    main()