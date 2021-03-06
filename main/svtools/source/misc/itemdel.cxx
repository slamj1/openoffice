/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_svtools.hxx"

#include <svtools/itemdel.hxx>
#include <vcl/svapp.hxx>
#include <tools/errcode.hxx>
#include <limits.h>

#include <svtools/svtdata.hxx>
#include <svl/svarray.hxx>
#include <svl/itempool.hxx>

// STATIC DATA -----------------------------------------------------------

DBG_NAME(SfxItemDesruptor_Impl);

// -----------------------------------------------------------------------

class SfxItemDesruptor_Impl
{
	SfxPoolItem *pItem;
	Link		 aLink;

private:
				 DECL_LINK( Delete, void * );
				 SfxItemDesruptor_Impl( const SfxItemDesruptor_Impl& ); // n.i.

public:
				 SfxItemDesruptor_Impl( SfxPoolItem *pItemToDesrupt );
				 ~SfxItemDesruptor_Impl();
};

SV_DECL_PTRARR( SfxItemDesruptorList_Impl, SfxItemDesruptor_Impl*, 4, 4 )

// ------------------------------------------------------------------------
SfxItemDesruptor_Impl::SfxItemDesruptor_Impl( SfxPoolItem *pItemToDesrupt ):
	pItem(pItemToDesrupt),
	aLink( LINK(this, SfxItemDesruptor_Impl, Delete) )
{
	DBG_CTOR(SfxItemDesruptor_Impl, 0);

	DBG_ASSERT( 0 == pItem->GetRefCount(), "desrupting pooled item" );
	pItem->SetKind( SFX_ITEMS_DELETEONIDLE );

	// im Idle abarbeiten
	GetpApp()->InsertIdleHdl( aLink, 1 );

	// und in Liste eintragen (damit geflusht werden kann)
	SfxItemDesruptorList_Impl* &rpList = ImpSvtData::GetSvtData().pItemDesruptList;
	if ( !rpList )
		rpList = new SfxItemDesruptorList_Impl;
	const SfxItemDesruptor_Impl *pThis = this;
	rpList->Insert( pThis, rpList->Count() );
}

// ------------------------------------------------------------------------
SfxItemDesruptor_Impl::~SfxItemDesruptor_Impl()
{
	DBG_DTOR(SfxItemDesruptor_Impl, 0);

	// aus Idle-Handler austragen
	GetpApp()->RemoveIdleHdl( aLink );

	// und aus Liste austragen
	SfxItemDesruptorList_Impl* &rpList = ImpSvtData::GetSvtData().pItemDesruptList;
	DBG_ASSERT( rpList, "no DesruptorList" );
	const SfxItemDesruptor_Impl *pThis = this;
	if ( rpList ) HACK(warum?)
		rpList->Remove( rpList->GetPos(pThis) );

	// reset RefCount (was set to SFX_ITEMS_SPECIAL before!)
	pItem->SetRefCount( 0 );
    //DBG_CHKOBJ( pItem, SfxPoolItem, 0 );
	delete pItem;
}

// ------------------------------------------------------------------------
IMPL_LINK( SfxItemDesruptor_Impl, Delete, void *, EMPTYARG )
{
	{DBG_CHKTHIS(SfxItemDesruptor_Impl, 0);}
	delete this;
	return 0;
}

// ------------------------------------------------------------------------
SfxPoolItem* DeleteItemOnIdle( SfxPoolItem* pItem )
{
	DBG_ASSERT( 0 == pItem->GetRefCount(), "deleting item in use" );
	new SfxItemDesruptor_Impl( pItem );
	return pItem;
}

// ------------------------------------------------------------------------
void DeleteOnIdleItems()
{
	SfxItemDesruptorList_Impl* &rpList
	 = ImpSvtData::GetSvtData().pItemDesruptList;
	if ( rpList )
	{
		sal_uInt16 n;
		while ( 0 != ( n = rpList->Count() ) )
			// Remove ist implizit im Dtor
			delete rpList->GetObject( n-1 );
		DELETEZ(rpList);
	}
}


