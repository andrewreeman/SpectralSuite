docker build . -t ss:latest
docker run -v $(pwd):/output ss:latest sh -c "cp -r build-unix/artefacts /output/unix-artefacts"
