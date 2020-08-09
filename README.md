# DawnCpp

## Draft template for C++ bot

### Build steps

- Clone the repo recursively `git clone --recurse-submodules git@github.com:heiseish/DawnCpp.git`
- Build the repo `make build`
- (Optional) Test the repo `make test`

### Run steps

- Create an `.env` file containing the following keys
```
DARKSKY_KEY=
NEWSAPI_KEY=
TELEGRAM_TOKEN=
TWITTER_ACCESS_TOKEN_KEY=
TWITTER_ACCESS_TOKEN_SECRET=
TWITTER_CONSUMER_KEY=
TWITTER_CONSUMER_SECRET=
SPDLOG_LEVEL=debug
```
- By default, the bot will interact with a backend python server that serves a dialoGPT-2 model and handles the conversation. Maybe released soon
- Run the bot with `make run`