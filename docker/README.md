# FlexFlow Docker
This folder contains the Dockerfiles and scripts that you can use to quickly run FlexFlow with no manual installation required. To use the containers, follow the steps below.

## Prerequisites
You will need a machine with a NVIDIA GPU, with drivers installed. You will also need to have Docker and the [Nvidia Container Toolkit](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/install-guide.html#getting-started) installed on the host machine.

## Downloading a pre-built package
The fastest way to run FlexFlow is to use one of the pre-built containers, which we update for each commit to the `inference` branch (the `inference` branch is currently ahead of the `master` branch). The available containers are the following, and can be found [at this link](https://github.com/orgs/flexflow/packages?repo_name=FlexFlow):

* [flexflow-cuda](https://github.com/orgs/flexflow/packages/container/package/flexflow-cuda): the pre-built version of FlexFlow targeting GPUs with a CUDA backend, for CUDA versions 11.1, 11.3, 11.7, 11.2, 11.5, 11.6, and 11.8.
* [flexflow-hip_rocm](https://github.com/orgs/flexflow/packages/container/package/flexflow-hip_rocm): the pre-built version of FlexFlow targeting GPUs with a HIP-ROCM backend.
* [flexflow-environment-cuda](https://github.com/orgs/flexflow/packages/container/package/flexflow-environment-cuda) and [flexflow-environment-hip_rocm](https://github.com/orgs/flexflow/packages/container/package/flexflow-environment-hip_rocm): these are the base layers for `flexflow-cuda` and `flexflow-hip_rocm`. The packages are used in CI or for internal use, and contain all the dependencies needed to build/run Flexflow.

The easiest way to download any of the Docker containers above is to call:

```
FF_GPU_BACKEND=<YOUR_GPU_BACKEND> cuda_version=<DESIRED_CUDA_VERSION> ./docker/pull.sh <CONTAINER_NAME>
```

where `CONTAINER_NAME` is `flexflow` (or `flexflow-environment`), and `FF_GPU_BACKEND`/`cuda_version` are optional environment variables you can use if you wish to download the docker image for a GPU backend and/or cuda version other than those installed on your machine (leaving these variables unset will let the script autodetect which version to download depending on your setup).

After downloading a container you can use the `run.sh` script to run it by following the instructions in the section below.

## Building a Docker container from scratch
If you prefer to build one of the Docker containers from scratch, you can do so with the help of the `build.sh` script. You can configure the build via the same environment variables that you'd use to configure a CMake build (refer to the [Installation guide](../INSTALL.md) and to the `config/config.linux` file). For example, to build for a CUDA backend, you can export `FF_GPU_BACKEND=cuda` (you can also omit this since `cuda` is the default value for `FF_GPU_BACKEND`). When building for the `cuda` backend, you can pick the CUDA version by setting the optional environment variable `cuda_version`, e.g.: `export cuda_version=11.8`. Leaving the `cuda_version` variable blank will let the script autodetect the CUDA version installed on the host machine, and build for that version. Setting the `cuda_version` env will have no effect when building for a GPU backend other than CUDA.

To build the FlexFlow container, run (the `flexflow` argument of the build script can be omitted):

```
FF_GPU_BACKEND=<YOUR_GPU_BACKEND> cuda_version=<DESIRED_CUDA_VERSION> ./docker/build.sh flexflow
```

If you only want to build the `flexflow-environment` image (the base layers of the `flexflow` container, used in CI and for other internal purposes), run:

```
FF_GPU_BACKEND=<YOUR_GPU_BACKEND> cuda_version=<DESIRED_CUDA_VERSION> ./docker/build.sh flexflow-environment
``` 

## Running a Docker container
After having either built or downloaded a Docker container by following the instructions above, you can run it with the following command (image name argument of the run script can be omitted). Once again, you can set the `FF_GPU_BACKEND` and `cuda_version` optional environment variables to run the docker image with the desired GPU backend and CUDA version. Leaving these variables unset will instruct the script to autodetect the GPU backend and CUDA version installed on the current machine and run the Docker container with it if available.

```
FF_GPU_BACKEND=<YOUR_GPU_BACKEND> cuda_version=<DESIRED_CUDA_VERSION> ./docker/run.sh --image_name flexflow
```

If you wish to run the `flexflow-environment` container, run:

```
FF_GPU_BACKEND=<YOUR_GPU_BACKEND> cuda_version=<DESIRED_CUDA_VERSION> ./docker/run.sh --image_name flexflow-environment
```

N.B.: If you don't have GPUs available on the machine, or you wish to run the docker image without attaching GPUs, you can set the environment variable `ATTACH_GPUS=false` before running the script.
