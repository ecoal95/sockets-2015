#include <string.h>

#include "protocol.h"
#include "tests.h"

TEST(parse_message_simple, {
    client_message_t result;
    const char* message = "HOLA abc";

    ASSERT(ERROR_NONE == parse_client_message(message, &result));
    ASSERT(MESSAGE_TYPE_HELLO == result.type);
    ASSERT(strcmp(result.content.uid, "abc") == 0);
})

TEST(parse_message_list_events, {
    client_message_t result;
    const char* message = "LISTAR EVENTOS";

    ASSERT(ERROR_NONE == parse_client_message(message, &result));
    ASSERT(MESSAGE_TYPE_EVENT_LIST == result.type);
})

TEST(parse_message_list_request, {
    client_message_t result;
    const char* message = "LISTAR abc ecdf";

    ASSERT(ERROR_NONE == parse_client_message(message, &result));
    ASSERT(MESSAGE_TYPE_ASSISTANCE_LIST == result.type);

    ASSERT(strcmp(result.content.assistance_list_info.uid, "abc") == 0);
    ASSERT(strcmp(result.content.assistance_list_info.event_id, "ecdf") == 0);

})

TEST(parse_message_assistance, {
    client_message_t result;
    const char* message = "FICHAR abc def 20-11-2015";

    ASSERT(ERROR_NONE == parse_client_message(message, &result));
    ASSERT(MESSAGE_TYPE_ASSISTANCE == result.type);

    ASSERT(strcmp(result.content.assistance_info.event_id, "abc") == 0);
    ASSERT(strcmp(result.content.assistance_info.uid, "def") == 0);

    ASSERT(result.content.assistance_info.datetime.tm_year == 2015 - 1900);
    ASSERT(result.content.assistance_info.datetime.tm_mon == 10);

    char buff[20];
    strftime(buff, 20, "%d-%m-%Y", &result.content.assistance_info.datetime);
    ASSERT(strcmp(buff, "20-11-2015") == 0);
})

TEST(parse_failure_invalid_type, {
    client_message_t result;
    const char* message = "ABC abc";

    ASSERT(ERROR_INVALID_MESSAGE_TYPE == parse_client_message(message, &result));
})

TEST(parse_failure_extra_content, {
    client_message_t result;
    const char* message = "HOLA abc ";

    ASSERT(ERROR_UNEXPECTED_CONTENT == parse_client_message(message, &result));
})

TEST(parse_failure_too_long_ident, {
    client_message_t result;
    const char* message = "HOLA aaaaaaaaaaaaaaaaaaaaa";

    ASSERT(ERROR_EXPECTED_VALID_UID == parse_client_message(message, &result));
})

TEST(parse_failure_missing_ident, {
    client_message_t result;
    const char* message = "LISTAR abc ";

    ASSERT(ERROR_EXPECTED_VALID_EVENT_ID == parse_client_message(message, &result));
})

TEST(parse_failure_invalid_date, {
    client_message_t result;
    const char* message = "FICHAR abc def fweafew";

    ASSERT(ERROR_EXPECTED_VALID_DATE == parse_client_message(message, &result));
})

TEST_MAIN({
    RUN_TEST(parse_message_simple);
    RUN_TEST(parse_message_list_events);
    RUN_TEST(parse_message_list_request);
    RUN_TEST(parse_message_assistance);
    RUN_TEST(parse_failure_invalid_type);
    RUN_TEST(parse_failure_extra_content);
    RUN_TEST(parse_failure_too_long_ident);
    RUN_TEST(parse_failure_missing_ident);
    RUN_TEST(parse_failure_invalid_date);
})
