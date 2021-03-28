#// ------------------------------------------------------------------------------------------
#/// \file                  Dockerfile to distribute the simulation
#/// \author                Sreejith.Naarakathil@gmail.com
#/// \date                  27/3/2021
#// ------------------------------------------------------------------------------------------

FROM alpine:3.12
RUN apk add build-base git && \
    git clone https://github.com/turbosree/vishwakarma.git && \
    make -C vishwakarma/IIoTFullSystem-Simulation/ clean && \
    make -C vishwakarma/IIoTFullSystem-Simulation/
COPY . .

# Run the process with a non-root user
USER 1000
ENTRYPOINT ["/vishwakarma/IIoTFullSystem-Simulation/RunSimulation"]
CMD ["1000000", "3", "100", "0"]
