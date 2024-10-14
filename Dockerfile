# Base image from the official mkp224o repository
FROM ghcr.io/cathugger/mkp224o:master

# Install necessary packages for compiling the C program
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    wamerican  # Install dictionary of English words

# Copy the custom C script into the container
COPY custom_mkp224o_filter.c /opt/mkp224o/custom_mkp224o_filter.c

# Compile the custom C program that filters onion addresses
RUN gcc -o /opt/mkp224o/custom_mkp224o_filter /opt/mkp224o/custom_mkp224o_filter.c

# Set the working directory
WORKDIR /opt/mkp224o

# Set the default command to generate .onion addresses and pipe to the custom filter
CMD ./mkp224o | ./custom_mkp224o_filter