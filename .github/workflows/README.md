# GitHub Actions Workflows

## Multi-Architecture Tests

**File**: `multi-arch-tests.yml`

### Purpose
Automatically tests dict-cpp on multiple CPU architectures to ensure cross-platform compatibility and endianness safety.

### Architectures Tested
- **x86_64 (amd64)**: Little-endian, native to GitHub runners
- **s390x**: Big-endian, emulated via QEMU

### Triggers
- Push to `main` or `develop` branches
- Pull requests to `main` or `develop` branches
- Manual trigger (Actions tab → Run workflow)

### Jobs

1. **test-native**
   - Runs on: Ubuntu latest (x86_64)
   - Tests: All unit tests + benchmarks
   - Duration: ~2-3 minutes

2. **test-big-endian**
   - Runs on: Ubuntu latest with QEMU s390x emulation
   - Tests: All unit tests + benchmarks
   - Duration: ~5-10 minutes (slower due to emulation)

3. **test-summary**
   - Aggregates results from both jobs
   - Reports overall pass/fail status

### Viewing Results

1. Navigate to the "Actions" tab in GitHub
2. Click on the latest workflow run
3. View individual job logs for detailed output

### Local Testing

Before pushing, you can run the same tests locally:

```bash
# Test native x86_64
docker build -t dict-cpp-native -f Dockerfile .
docker run --rm dict-cpp-native

# Test big-endian s390x
docker buildx build --platform linux/s390x -t dict-cpp-bigendian -f Dockerfile.bigendian . --load
docker run --platform linux/s390x --rm dict-cpp-bigendian
```

### Expected Results

Both jobs should show:
- ✅ 29/29 unit tests passed
- ✅ All benchmarks completed successfully
- ✅ dict-cpp faster than nlohmann/json in most operations

### Troubleshooting

If workflows fail:

1. Check the job logs in the Actions tab
2. Look for architecture-specific errors
3. Verify Docker and QEMU setup steps completed successfully
4. Test locally using the commands above

For more details, see [`docs/MULTI_ARCH_TESTING.md`](../../docs/MULTI_ARCH_TESTING.md).


