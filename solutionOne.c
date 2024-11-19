//solution1
// • Uses an integer variable readers
// o Shared between readers
// o Counts the number of readers accessing or trying to access the shared
// resource
// o Initially 0

// • Uses two semaphores:
// • resource: to synchronize access to the shared
// resource between readers and writers

// • mutex: to avoid race condition with the readers
// variable between readers

// • First reader locks resource, if available
// o Locking out writers
// • Subsequent readers do not need to lock it
// • Several readers can read simultaneously
// • A writer needs to lock the resource before
// accessing it
// o Locking both reader and writes
// • A stream of readers can indefinitely lock writers
// out

// Writer 
// <Entry Section>
// resource.wait()

// <Exit Section>
// resource.signal()

// Reader
// <Entry Section>
// mutex.wait()
// readers++
// if (readers == 1)
// resource.wait()
// mutex.signal()

// <Exit Section>
// mutex.wait()
// readers--
// If (readers == 0)
// resource.signal()
// mutex.signal()