FROM resin/raspberrypi3-debian:stretch

# Enable systemd
ENV INITSYSTEM on

# Default to using 2 make jobs, which is a good default for CI.
ARG MAKEFLAGS=-j2

# Copy repository to container
COPY . /fotobox/
WORKDIR /fotobox/

# Install development tools
RUN sudo apt-get update && sudo apt-get -y install \
  wiringpi \
  qt5-default \
  qttools5-dev-tools \
  build-essential \
  ccache \
  git

# Build FotoBox
RUN qmake -v && qmake -r
RUN make

# Compress application
RUN tar -czvf fotobox.tar.gz ./fotobox
