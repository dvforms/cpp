node("cmake") {
    checkout scm
    def cmake = sh returnStdout: true, script: 'which cmake3 cmake | head -n1'
    def ctest = sh returnStdout: true, script: 'which ctest3 ctest | head -n1'
    withEnv(["CMAKE=${cmake}", "CTEST=${ctest}", "IGNORE_EXIT_CODE=1"]) {
        dir("build") {
            cache(caches: [[$class: 'ArbitraryFileCache', excludes: '', includes: '**/*.zip,**/*.tar.gz', path: 'external']], maxCacheSize: 100) {
                sh 'nice ${CMAKE} -G Ninja ../ -DBUILD_TESTING=ON -DVALGRIND_XML=ON -DCPACK_BINARY_RPM=ON -DCMAKE_BUILD_TYPE=Debug'
                sh 'nice ${CMAKE} --build . -- -j2 -l15 all'
                catchError {
                    sh 'nice ${CTEST} --verbose .'
                }
                sh '${CMAKE} --build . -- package'
                junit '**/*-unit.xml'
                archiveArtifacts '**/*valgrind.xml'
                step([$class        : 'XUnitBuilder',
                      testTimeMargin: '3000',
                      thresholdMode : 1,
                      thresholds    : [
                              [$class: 'FailedThreshold', failureNewThreshold: '', failureThreshold: '', unstableNewThreshold: '', unstableThreshold: '0'],
                              [$class: 'SkippedThreshold', failureNewThreshold: '', failureThreshold: '', unstableNewThreshold: '', unstableThreshold: '0']
                      ],
                      tools         : [
                              [$class               : 'ValgrindJunitHudsonTestType',
                               deleteOutputFiles    : true,
                               failIfNotNew         : true,
                               pattern              : '**/*valgrind.xml',
                               skipNoTestFiles      : true,
                               stopProcessingIfError: true]
                      ]
                ])
            }
        }
    }
}
