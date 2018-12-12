# Sarmata ASR gRPC C++ client Changelog

## [2.2.0] - 2018-12-12
### Added
- Support for setting gRPC deadline (how long the client is willing to wait for a reply from the server).

## [2.1.1] - 2018-03-21
### Changed
- `SarmataSessionConfig` to have a field for sample rate and to store service settings in a map instead of a string.

## [2.1.0] - 2018-03-08
### Added
- cmake build system.

### Changed
- Session interface implementation to simple workflow.

### Removed
- scons build system.

## [2.0.0] - 2018-03-05
### Changed
- Updated to new API.
