# Create an API file

### Synopsis
Other files are going to access calls from this process, it'll be nice if there was an API that has that. This could be done by moving most of the parts from the main net.c into the net_api.c

### Description
If City were to check if the platform has internet access, it can call a function within Net and it can return true for has internt access or false for needs internet access
