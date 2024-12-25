docker build . -t ss:latest
rm -rf linux-artefacts
mkdir linux-artefacts
docker run -v $(pwd):/output ss:latest sh -c "cp -r /ss/build-linux/artefacts/* /output/linux-artefacts"
