/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2007 Richard Hughes <richard@hughsie.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <sys/types.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */

#include <glib/gi18n.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>

#include "pk-debug.h"
#include "pk-task.h"
#include "pk-task-common.h"

static void     pk_task_class_init	(PkTaskClass *klass);
static void     pk_task_init		(PkTask      *task);
static void     pk_task_finalize	(GObject     *object);

#define PK_TASK_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), PK_TYPE_TASK, PkTaskPrivate))

struct PkTaskPrivate
{
	gboolean		 whatever_you_want;
};

static guint signals [PK_TASK_LAST_SIGNAL] = { 0, };

G_DEFINE_TYPE (PkTask, pk_task, G_TYPE_OBJECT)

/**
 * pk_task_get_updates:
 **/
gboolean
pk_task_get_updates (PkTask *task)
{
	g_return_val_if_fail (task != NULL, FALSE);
	g_return_val_if_fail (PK_IS_TASK (task), FALSE);

	if (pk_task_assign (task) == FALSE) {
		return FALSE;
	}

	pk_task_change_job_status (task, PK_TASK_STATUS_QUERY);
	//dlopend_get_updates
if (0)	pk_task_change_percentage_complete (task, 0);
if (0)	pk_task_finished (task, PK_TASK_EXIT_SUCCESS);

	return TRUE;
}

/**
 * pk_task_update_system:
 **/
gboolean
pk_task_update_system (PkTask *task)
{
	g_return_val_if_fail (task != NULL, FALSE);
	g_return_val_if_fail (PK_IS_TASK (task), FALSE);

	if (pk_task_assign (task) == FALSE) {
		return FALSE;
	}

	pk_task_change_job_status (task, PK_TASK_STATUS_UPDATE);

	//pk_module_update()

	return TRUE;
}

/**
 * pk_task_find_packages:
 **/
gboolean
pk_task_find_packages (PkTask *task, const gchar *search)
{
	g_return_val_if_fail (task != NULL, FALSE);
	g_return_val_if_fail (PK_IS_TASK (task), FALSE);

	if (pk_task_assign (task) == FALSE) {
		return FALSE;
	}

	pk_task_change_job_status (task, PK_TASK_STATUS_QUERY);

	return TRUE;
}

/**
 * pk_task_get_dependencies:
 **/
gboolean
pk_task_get_dependencies (PkTask *task, const gchar *package)
{
	g_return_val_if_fail (task != NULL, FALSE);
	g_return_val_if_fail (PK_IS_TASK (task), FALSE);

	if (pk_task_assign (task) == FALSE) {
		return FALSE;
	}

	pk_task_change_job_status (task, PK_TASK_STATUS_QUERY);

	return TRUE;
}

/**
 * pk_task_remove_packages:
 **/
gboolean
pk_task_remove_packages (PkTask *task, const gchar **packages)
{
	g_return_val_if_fail (task != NULL, FALSE);
	g_return_val_if_fail (PK_IS_TASK (task), FALSE);

	if (pk_task_assign (task) == FALSE) {
		return FALSE;
	}

	pk_task_change_job_status (task, PK_TASK_STATUS_REMOVE);

	return TRUE;
}

/**
 * pk_task_remove_packages_with_dependencies:
 **/
gboolean
pk_task_remove_packages_with_dependencies (PkTask *task, const gchar **packages)
{
	g_return_val_if_fail (task != NULL, FALSE);
	g_return_val_if_fail (PK_IS_TASK (task), FALSE);

	if (pk_task_assign (task) == FALSE) {
		return FALSE;
	}

	pk_task_change_job_status (task, PK_TASK_STATUS_REMOVE);

	return TRUE;
}

/**
 * pk_task_install_packages:
 **/
gboolean
pk_task_install_packages (PkTask *task, const gchar **packages)
{
	g_return_val_if_fail (task != NULL, FALSE);
	g_return_val_if_fail (PK_IS_TASK (task), FALSE);

	if (pk_task_assign (task) == FALSE) {
		return FALSE;
	}

	pk_task_change_job_status (task, PK_TASK_STATUS_INSTALL);

	//pk_backend_install (packages, FALSE);

	return TRUE;
}

/**
 * pk_task_cancel_job_try:
 **/
gboolean
pk_task_cancel_job_try (PkTask *task)
{
	g_return_val_if_fail (task != NULL, FALSE);
	g_return_val_if_fail (PK_IS_TASK (task), FALSE);

	/* check to see if we have an action */
	if (task->assigned == FALSE) {
		g_warning ("Not assigned");
		return FALSE;
	}
	/* try to cancel action */
	if (task->status != PK_TASK_STATUS_QUERY) {
		g_warning ("cannot cancel as not query");
		return FALSE;
	}

	/* close process */
	//pk_backend_cancel ();

	return TRUE;
}

/**
 * pk_task_class_init:
 **/
static void
pk_task_class_init (PkTaskClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = pk_task_finalize;
	pk_task_setup_signals (object_class, signals);
	g_type_class_add_private (klass, sizeof (PkTaskPrivate));
}

/**
 * pk_task_init:
 **/
static void
pk_task_init (PkTask *task)
{
	task->priv = PK_TASK_GET_PRIVATE (task);
	task->signals = signals;
	pk_task_clear (task);
}

/**
 * pk_task_finalize:
 **/
static void
pk_task_finalize (GObject *object)
{
	PkTask *task;
	g_return_if_fail (object != NULL);
	g_return_if_fail (PK_IS_TASK (object));
	task = PK_TASK (object);
	g_return_if_fail (task->priv != NULL);
	G_OBJECT_CLASS (pk_task_parent_class)->finalize (object);
}

/**
 * pk_task_new:
 **/
PkTask *
pk_task_new (void)
{
	PkTask *task;
	task = g_object_new (PK_TYPE_TASK, NULL);
	return PK_TASK (task);
}

