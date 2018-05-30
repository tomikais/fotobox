FROM resin/raspberrypi3-debian:stretch

# Enable systemd
ENV INITSYSTEM on

# Install development tools
RUN sudo apt-get update && sudo apt-get -y install \
  wiringpi \
  qt5-default \
  qttools5-dev-tools \
  build-essential \
  ccache \
  git

# Clone Repo
RUN rm -f -r ./fotobox
RUN git clone https://gitlab.com/tomikais/fotobox.git

# Build FotoBox
RUN cd fotobox && qmake -v
RUN cd fotobox && qmake -r
RUN cd fotobox && make

# Compress apllication
RUN tar -czvf fotobox.tar.gz ./fotobox/fotobox
