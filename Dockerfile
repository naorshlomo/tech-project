## build stage
FROM abeimler/simple-cppbuilder as build
COPY . .
CMD ["./docker-build.sh"]
