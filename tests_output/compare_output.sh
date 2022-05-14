search_dir=.
for entry in "$search_dir"/*
do
    if [[ $entry == *.txt ]] && [[ -f "verified_output/${entry:2}" ]]
    then
        if [[ $(diff "${entry:2}" "verified_output/${entry:2}") ]]
        then
            echo "${entry:2} differed from verified version."
        fi
    fi
done