# https://docs.resin.io/reference/base-images/resin-base-images/
# https://hub.docker.com/r/resin/raspberrypi3-debian/
FROM resin/raspberrypi3-debian:stretch

# Enable systemd
ENV INITSYSTEM on

# Install build tools, frameworks, ...
RUN sudo apt-get update && sudo apt-get -y install \
  wiringpi \
  qt5-default \
  qttools5-dev-tools \
  build-essential \
  ccache \
  git


# Copy repository to container
COPY . /fotobox/
WORKDIR /fotobox/

# Show qmake Version / generate project / build FotoBox / compress artifact
CMD qmake -v && qmake -r && make -j2 && tar -czvf fotobox.tar.gz /output
