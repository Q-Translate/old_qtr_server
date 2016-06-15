#!/bin/bash

curl -k -i -H "Content-type: application/json"  \
     -X POST https://dev.qtranslate.org/api/
report/statistics.json  \
     -d '{"lng": "sq"}'
