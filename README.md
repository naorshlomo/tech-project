# tech-project
docker-compose up --build -d dev-image && docker exec -it tech-project_dev-image_1 /bin/bash
docker-compose up --build --scale worker-image=1 worker-image
docker-compose up --build --scale worker-image=3 worker-image