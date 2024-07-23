FROM mcr.microsoft.com/devcontainers/universal:2

## Install cJSON Dependency
RUN cd /tmp && \
    git clone  --depth=1  https://github.com/DaveGamble/cJSON.git && \
    mkdir -p cJSON/build && cd cJSON/build && \
    cmake .. && make && make install && \
    ldconfig -n /usr/local/lib && \
    cd /tmp && \
    rm -r /tmp/cJSON

## Install Acutest Dependency
RUN mkdir -p /usr/local/include/acutest && \
    curl https://raw.githubusercontent.com/mity/acutest/master/include/acutest.h \
         -o /usr/local/include/acutest/acutest.h