#https://docs.resin.io/reference/base-images/resin-base-images/
#https://hub.docker.com/r/resin/raspberrypi3-debian/
FROM resin/raspberrypi3-debian:stretch

#install frameworks and build tools
RUN sudo apt-get update && sudo apt-get -y install \
  wiringpi \
  qt5-default \
  qttools5-dev-tools \
  build-essential \
  ccache


#copy repository to container
COPY . /fotobox/
WORKDIR /fotobox/

#show qmake version / generate project / build FotoBox / compress artifact
CMD qmake -v && qmake -r && make -j2 && tar -czvf ./FotoBox_RasPi.tar.gz ./FotoBox
