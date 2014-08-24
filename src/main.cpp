#include <iostream>
#include <memory>
#include <thread>
#include <future>
#include <vector>
#include <algorithm>
#include <boost/lexical_cast.hpp>

#include "CKeyFactory.hpp"

using namespace std;
using namespace BitHack;

typedef vector<CKey>                 resultType;
typedef shared_ptr<resultType>       resultPointer;
typedef shared_future<resultPointer> resultFuture;

resultPointer thread_generator(const size_t count) {
    auto result = make_shared<resultType>();
    result->resize(count);

    CKeyFactory factory;
    factory.createKey();

    for(auto i = begin(*result); i < end(*result); ++i) {
        *i = factory.nextKey();
    }

    return result;
}

int main(int argc, char* argv[]) {

    int thread_count = 8;
    int keys_count   = 100000;

    if (argc == 3) {
        thread_count = boost::lexical_cast<int>(argv[1]);
        keys_count   = boost::lexical_cast<int>(argv[2]);
    }

    thread_count = min(thread_count, keys_count);

    int keys_per_thread         = keys_count/thread_count;
    int add_keys_to_last_thread = keys_count % thread_count;

    cerr << "Thread count: \t\t\t"            << thread_count            << endl;
    cerr << "Keys per thread: \t\t"           << keys_per_thread         << endl;
    cerr << "Add keys to the last thread: \t" << add_keys_to_last_thread << endl;

    vector< resultFuture >    results;

    for(auto i = 0; i < thread_count; ++i) {

        if(i+1 == thread_count) keys_per_thread+=add_keys_to_last_thread;

        auto customer_generator = bind(thread_generator, keys_per_thread);
        auto task = packaged_task<resultPointer()>(customer_generator);
        results.push_back(task.get_future().share());
        thread(std::move(task)).detach();
        cerr << "Thread " << i+1 << " is running for generate " << keys_per_thread << " keys"<< endl;
        cerr.flush();
    }

    while(true) {
        static auto i = 0;
        if(all_of(begin(results), end(results), [](const resultFuture f) {
            return f.wait_for(std::chrono::seconds(0)) == future_status::ready;
        }
        )) {
            break;
        }
        cerr << ++i << " ";
        cerr.flush();
        sleep(1);
    }
    cerr << endl;

    auto result_count = 0;
    for_each(begin(results), end(results), [&result_count](const resultFuture r){result_count += r.get()->size();});
    cerr << "Generated keys: " << result_count << endl;
    cerr << "Writte keys...";

    for_each(begin(results), end(results),
            [](const resultFuture f) {
                for_each(begin(*f.get()), end(*f.get()),
                        [](const CKey key) {
                    cout << key;
                });
    });
    cout.flush();
    cerr << "Good bye!" << endl;

    return (0);
}
