/*
** Zabbix
** Copyright (C) 2001-2020 Zabbix SIA
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#include "common.h"
#include "db.h"
#include "dbupgrade.h"

/*
 * 5.2 development database patches
 */

#ifndef HAVE_SQLITE3

extern unsigned char	program_type;

static int	DBpatch_5010000(void)
{
	const ZBX_FIELD	field = {"default_lang", "en_GB", NULL, NULL, 5, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010001(void)
{
	const ZBX_FIELD	field = {"lang", "default", NULL, NULL, 7, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBmodify_field_type("users", &field, NULL);
}

static int	DBpatch_5010002(void)
{
	if (ZBX_DB_OK > DBexecute("update users set lang='default',theme='default' where alias='guest'"))
		return FAIL;

	return SUCCEED;
}

static int	DBpatch_5010003(void)
{
	if (0 == (program_type & ZBX_PROGRAM_TYPE_SERVER))
		return SUCCEED;

	if (ZBX_DB_OK > DBexecute("delete from profiles where idx in ('web.latest.toggle','web.latest.toggle_other')"))
		return FAIL;

	return SUCCEED;
}

static int	DBpatch_5010004(void)
{
	DB_ROW		row;
	DB_RESULT	result;
	int		ret = SUCCEED;

	if (0 == (program_type & ZBX_PROGRAM_TYPE_SERVER))
		return SUCCEED;

	result = DBselect("select userid from profiles where idx='web.latest.sort' and value_str='lastclock'");

	while (NULL != (row = DBfetch(result)))
	{
		if (ZBX_DB_OK > DBexecute(
			"delete from profiles"
			" where userid='%s'"
				" and idx in ('web.latest.sort','web.latest.sortorder')", row[0]))
		{
			ret = FAIL;
			break;
		}
	}
	DBfree_result(result);

	return ret;
}

static int	DBpatch_5010005(void)
{
	const ZBX_FIELD	field = {"default_timezone", "system", NULL, NULL, 50, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010006(void)
{
	const ZBX_FIELD	field = {"timezone", "default", NULL, NULL, 50, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBadd_field("users", &field);
}

static int	DBpatch_5010007(void)
{
	const ZBX_FIELD	field = {"login_attempts", "5", NULL, NULL, 0, ZBX_TYPE_INT, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010008(void)
{
	const ZBX_FIELD	field = {"login_block", "30s", NULL, NULL, 32, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010009(void)
{
	const ZBX_FIELD	field = {"show_technical_errors", "0", NULL, NULL, 0, ZBX_TYPE_INT, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010010(void)
{
	const ZBX_FIELD	field = {"validate_uri_schemes", "1", NULL, NULL, 0, ZBX_TYPE_INT, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010011(void)
{
	const ZBX_FIELD	field = {"uri_valid_schemes", "http,https,ftp,file,mailto,tel,ssh", NULL, NULL, 255, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010012(void)
{
	const ZBX_FIELD	field = {"x_frame_options", "SAMEORIGIN", NULL, NULL, 255, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010013(void)
{
	const ZBX_FIELD	field = {"iframe_sandboxing_enabled", "1", NULL, NULL, 0, ZBX_TYPE_INT, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010014(void)
{
	const ZBX_FIELD	field = {"iframe_sandboxing_exceptions", "", NULL, NULL, 255, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010015(void)
{
	const ZBX_FIELD	field = {"max_overview_table_size", "50", NULL, NULL, 0, ZBX_TYPE_INT, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010016(void)
{
	const ZBX_FIELD	field = {"history_period", "24h", NULL, NULL, 32, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010017(void)
{
	const ZBX_FIELD	field = {"period_default", "1h", NULL, NULL, 32, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010018(void)
{
	const ZBX_FIELD	field = {"max_period", "2y", NULL, NULL, 32, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010019(void)
{
	const ZBX_FIELD	field = {"socket_timeout", "3s", NULL, NULL, 32, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010020(void)
{
	const ZBX_FIELD	field = {"connect_timeout", "3s", NULL, NULL, 32, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010021(void)
{
	const ZBX_FIELD	field = {"media_type_test_timeout", "65s", NULL, NULL, 32, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010022(void)
{
	const ZBX_FIELD	field = {"script_timeout", "60s", NULL, NULL, 32, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010023(void)
{
	const ZBX_FIELD	field = {"item_test_timeout", "60s", NULL, NULL, 32, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010024(void)
{
	const ZBX_FIELD	field = {"session_key", "", NULL, NULL, 32, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBadd_field("config", &field);
}

static int	DBpatch_5010025(void)
{
	const ZBX_FIELD field = {"value", "", NULL, NULL, 2048, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBmodify_field_type("hostmacro", &field, NULL);
}

static int	DBpatch_5010026(void)
{
	const ZBX_FIELD field = {"value", "", NULL, NULL, 2048, ZBX_TYPE_CHAR, ZBX_NOTNULL, 0};

	return DBmodify_field_type("globalmacro", &field, NULL);
}

static int	DBpatch_5010027(void)
{
	const ZBX_FIELD	old_field = {"data", "", NULL, NULL, 0, ZBX_TYPE_SHORTTEXT, ZBX_NOTNULL, 0};
	const ZBX_FIELD	field = {"data", "", NULL, NULL, 0, ZBX_TYPE_TEXT, ZBX_NOTNULL, 0};

	return DBmodify_field_type("task_data", &field, &old_field);
}

static int	DBpatch_5010028(void)
{
	const ZBX_FIELD	old_field = {"info", "", NULL, NULL, 0, ZBX_TYPE_SHORTTEXT, ZBX_NOTNULL, 0};
	const ZBX_FIELD	field = {"info", "", NULL, NULL, 0, ZBX_TYPE_TEXT, ZBX_NOTNULL, 0};

	return DBmodify_field_type("task_result", &field, &old_field);
}

static int	DBpatch_5010029(void)
{
	const ZBX_FIELD	old_field = {"params", "", NULL, NULL, 0, ZBX_TYPE_SHORTTEXT, ZBX_NOTNULL, 0};
	const ZBX_FIELD	field = {"params", "", NULL, NULL, 0, ZBX_TYPE_TEXT, ZBX_NOTNULL, 0};

	return DBmodify_field_type("items", &field, &old_field);
}

static int	DBpatch_5010030(void)
{
	const ZBX_FIELD	old_field = {"description", "", NULL, NULL, 0, ZBX_TYPE_SHORTTEXT, ZBX_NOTNULL, 0};
	const ZBX_FIELD	field = {"description", "", NULL, NULL, 0, ZBX_TYPE_TEXT, ZBX_NOTNULL, 0};

	return DBmodify_field_type("items", &field, &old_field);
}

static int	DBpatch_5010031(void)
{
	const ZBX_FIELD	old_field = {"posts", "", NULL, NULL, 0, ZBX_TYPE_SHORTTEXT, ZBX_NOTNULL, 0};
	const ZBX_FIELD	field = {"posts", "", NULL, NULL, 0, ZBX_TYPE_TEXT, ZBX_NOTNULL, 0};

	return DBmodify_field_type("items", &field, &old_field);
}

static int	DBpatch_5010032(void)
{
	const ZBX_FIELD	old_field = {"headers", "", NULL, NULL, 0, ZBX_TYPE_SHORTTEXT, ZBX_NOTNULL, 0};
	const ZBX_FIELD	field = {"headers", "", NULL, NULL, 0, ZBX_TYPE_TEXT, ZBX_NOTNULL, 0};

	return DBmodify_field_type("items", &field, &old_field);
}

#endif

DBPATCH_START(5010)

/* version, duplicates flag, mandatory flag */

DBPATCH_ADD(5010000, 0, 1)
DBPATCH_ADD(5010001, 0, 1)
DBPATCH_ADD(5010002, 0, 1)
DBPATCH_ADD(5010003, 0, 1)
DBPATCH_ADD(5010004, 0, 1)
DBPATCH_ADD(5010005, 0, 1)
DBPATCH_ADD(5010006, 0, 1)
DBPATCH_ADD(5010007, 0, 1)
DBPATCH_ADD(5010008, 0, 1)
DBPATCH_ADD(5010009, 0, 1)
DBPATCH_ADD(5010010, 0, 1)
DBPATCH_ADD(5010011, 0, 1)
DBPATCH_ADD(5010012, 0, 1)
DBPATCH_ADD(5010013, 0, 1)
DBPATCH_ADD(5010014, 0, 1)
DBPATCH_ADD(5010015, 0, 1)
DBPATCH_ADD(5010016, 0, 1)
DBPATCH_ADD(5010017, 0, 1)
DBPATCH_ADD(5010018, 0, 1)
DBPATCH_ADD(5010019, 0, 1)
DBPATCH_ADD(5010020, 0, 1)
DBPATCH_ADD(5010021, 0, 1)
DBPATCH_ADD(5010022, 0, 1)
DBPATCH_ADD(5010023, 0, 1)
DBPATCH_ADD(5010024, 0, 1)
DBPATCH_ADD(5010025, 0, 1)
DBPATCH_ADD(5010026, 0, 1)
DBPATCH_ADD(5010027, 0, 1)
DBPATCH_ADD(5010028, 0, 1)
DBPATCH_ADD(5010029, 0, 1)
DBPATCH_ADD(5010030, 0, 1)
DBPATCH_ADD(5010031, 0, 1)
DBPATCH_ADD(5010032, 0, 1)

DBPATCH_END()
