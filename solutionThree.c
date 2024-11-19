//solution3

// Uses two shared variables:
// • readers: counts the number of trying readers

// • writers: counts the number of trying writers

// • Uses two shared semaphores:
// • mutex: synchronizes access to readers and
// writers
// • resource: controls access to CS

// Writer 
// <Entry Section>
// mutex.wait()
// writers++
// mutex.signal()
// resource.wait()

// <Exit Section>
// mutex.wait()
// writers--
// mutex.signal()
// resource.signal()

// Reader
// <Entry Section>
// mutex.wait()
// if (writers > 0 or readers ==0) {
// mutex.signal()
// resource.wait()
// mutex.wait()
// }
// readers++
// mutex.signal()

// <Exit Section>
// mutex.wait()
// readers--
// If (readers == 0) resource.signal()
// mutex.signal()