[![MIT licensed](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://travis-ci.com/GPUOpen-ProfessionalCompute-Libraries/rpp.svg?branch=master)](https://travis-ci.com/GPUOpen-ProfessionalCompute-Libraries/rpp)

# ROCm Performance Primitives Library

AMD ROCm Performance Primitives (**RPP**) library is a comprehensive high-performance computer vision library for AMD processors with `HIP`/`OpenCL`/`CPU` back-ends.

## Latest Release

[![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/GPUOpen-ProfessionalCompute-Libraries/rpp?style=for-the-badge)](https://github.com/GPUOpen-ProfessionalCompute-Libraries/rpp/releases)

## Documentation

Run the steps below to build documentation locally.

```bash
cd docs

pip3 install -r .sphinx/requirements.txt

python3 -m sphinx -T -E -b html -d _build/doctrees -D language=en . _build/html
```