# how to use
# docker build -t star .
# make sure docker has been installed
# FROM rockylinux:8
FROM ubuntu

ARG HOME_DIR=/root
ARG GIT_SOURCE=github

ENV LANG=en_US.UTF-8

# install rpm
# note: gcc-c++ in rockylinux 8 and gcc-g++ in rockylinux 9. use `dnf groupinfo "Development Tools"` to list the tools
RUN apt-get update
RUN apt-get install -y zip make cmake g++ git libjemalloc-dev libboost-dev libgoogle-glog-dev

# prepare env
WORKDIR ${HOME_DIR}
RUN echo alias ls=\"ls --color=auto\" >> .bashrc
RUN echo "export LD_LIBRARY_PATH=/usr/local/lib64:\$LD_LIBRARY_PATH" >> .bashrc

# # clone star code
# RUN mkdir -p ${HOME_DIR}/source
# WORKDIR ${HOME_DIR}/source
# # RUN git clone https://${GIT_SOURCE}.com:YunWorkshop/star.git
# RUN git clone https://gitee.com/zhahantian/star.git
# RUN echo "mkdir -p build && cd build && cmake .. && make -j" > ${HOME_DIR}/source/star/build.sh && chmod +x ${HOME_DIR}/source/star/build.sh
# RUN mkdir -p ${HOME_DIR}/source/star/build
# WORKDIR ${HOME_DIR}/source/star/build
# RUN cmake -B ${HOME_DIR}/source/star/build ${HOME_DIR}/source/star/ \
#     && make -j4 \
#     && rm -rf ${HOME_DIR}/source/star/build

# WORKDIR ${HOME_DIR}

ENTRYPOINT tail -f /dev/null
