#https://docs.resin.io/reference/base-images/resin-base-images/
#https://hub.docker.com/r/resin/raspberrypi3-debian/
FROM resin/raspberrypi3-debian:stretch

#install frameworks and build tools
RUN sudo apt-get update -qq && sudo apt-get install -qq \
  wiringpi \
  qt5-default \
  qttools5-dev-tools \
  build-essential \
  ccache


#copy repository to container
COPY . .

#show qmake version / generate project / build FotoBox / compress artifact / copy to host (mounted directory '--volume')
CMD qmake -v && qmake -r && make -j2 && tar -cvzf ./FotoBox_RasPi.tar.gz ./FotoBox && cp ./FotoBox_RasPi.tar.gz ./artifact/
