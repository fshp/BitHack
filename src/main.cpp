#include <iostream>
#include <memory>
#include <thread>
#include <future>
#include <chrono>
#include <unistd.h>
#include <vector>
#include <algorithm>

#include "CKey.hpp"
#include "CKeyFactory.hpp"

using namespace std;
using namespace BitHack;

typedef vector<CKey>                 resultType;
typedef shared_ptr<resultType>       resultPointer;
typedef shared_future<resultPointer> resultFuture;

auto thread_generator(const size_t count) {
    auto result = make_shared<resultType>();
    result->resize(count);

    CKeyFactory factory;
    factory.createKey();

    for(auto i = begin(*result); i < end(*result); ++i) {
        *i = factory.nextKey();
    }

    return result;
}

int main() {

    const auto thread_count = 8;
    const auto keys_count   = 100000;

    cerr << "Thread count: " << thread_count << endl;
    cerr << "Keys per thread: " << keys_count << endl;

    vector< resultFuture >    results;

    for(auto i = 0; i < thread_count; ++i) {
        auto customer_generator = bind(thread_generator, keys_count);
        auto task = packaged_task<resultPointer()>(customer_generator);
        results.push_back(task.get_future().share());
        thread(std::move(task)).detach();
        cerr << "Thread " << i+1 << " is running" << endl;
        cerr.flush();
    }

    while(true) {
        static auto i = 0;
        if(all_of(begin(results), end(results), [](const auto f) {
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
    for_each(begin(results), end(results), [&result_count](auto r){result_count += r.get()->size();});
    cerr << "Generated keys: " << result_count << endl;
    cerr << "Writte keys...";

    for_each(begin(results), end(results),
            [](auto f) {
                for_each(begin(*f.get()), end(*f.get()),
                        [](auto key) {
                    cout << key;
                });
    });

    cerr << "Good bye!" << endl;

/*    auto f1 = t.get_future();
    thread(std::move(t)).detach();

    for(auto i = 0; i < 100000; ++i) {
        cout << i;
        cout.flush();
        if(f1.wait_for(std::chrono::seconds(1)) == future_status::ready) break;
    }
    cout << endl;

    f1.wait();
    auto result = f1.get();

    cout << "Gen keys: " << result->size() << endl;*/

    return (0);
}
