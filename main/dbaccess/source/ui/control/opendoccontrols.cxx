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
#include "precompiled_dbui.hxx"

#ifndef DBACCESS_SOURCE_UI_INC_OPENDOCCONTROLS_HXX
#include "opendoccontrols.hxx"
#endif

/** === begin UNO includes === **/
#ifndef _COM_SUN_STAR_UNO_SEQUENCE_HXX_
#include <com/sun/star/uno/Sequence.hxx>
#endif
#ifndef _COM_SUN_STAR_BEANS_PROPERTYVALUE_HPP_
#include <com/sun/star/beans/PropertyValue.hpp>
#endif
#ifndef _COM_SUN_STAR_LANG_XMULTISERVICEFACTORY_HPP_
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#endif
#ifndef _COM_SUN_STAR_CONTAINER_XNAMEACCESS_HPP_
#include <com/sun/star/container/XNameAccess.hpp>
#endif
#ifndef _COM_SUN_STAR_UI_XMODULEUICONFIGURATIONMANAGERSUPPLIER_HPP_
#include <com/sun/star/ui/XModuleUIConfigurationManagerSupplier.hpp>
#endif
#ifndef _COM_SUN_STAR_UI_XUICONFIGURATIONMANAGER_HPP_
#include <com/sun/star/ui/XUIConfigurationManager.hpp>
#endif
#ifndef _COM_SUN_STAR_GRAPHIC_XGRAPHIC_HPP_
#include <com/sun/star/graphic/XGraphic.hpp>
#endif
#ifndef _COM_SUN_STAR_UI_XIMAGEMANAGER_HPP_
#include <com/sun/star/ui/XImageManager.hpp>
#endif
/** === end UNO includes === **/

#ifndef _COMPHELPER_PROCESSFACTORY_HXX_
#include <comphelper/processfactory.hxx>
#endif
#ifndef _SV_GRAPH_HXX
#include <vcl/graph.hxx>
#endif
#ifndef _SV_HELP_HXX
#include <vcl/help.hxx>
#endif
#ifndef INCLUDED_SVTOOLS_HISTORYOPTIONS_HXX
#include <unotools/historyoptions.hxx>
#endif
#ifndef _COMPHELPER_SEQUENCEASHASHMAP_HXX_
#include <comphelper/sequenceashashmap.hxx>
#endif
#ifndef _URLOBJ_HXX
#include <tools/urlobj.hxx>
#endif
#ifndef SVTOOLS_FILENOTATION_HXX
#include <svl/filenotation.hxx>
#endif

//........................................................................
namespace dbaui
{
//........................................................................

    namespace
    {
        using ::com::sun::star::uno::Reference;
        using ::com::sun::star::uno::Exception;
        using ::com::sun::star::uno::Sequence;
        using ::com::sun::star::uno::UNO_QUERY_THROW;
        using ::com::sun::star::container::XNameAccess;
        using ::com::sun::star::lang::XMultiServiceFactory;
        using ::com::sun::star::beans::PropertyValue;
        using ::com::sun::star::ui::XModuleUIConfigurationManagerSupplier;
        using ::com::sun::star::ui::XUIConfigurationManager;
        using ::com::sun::star::ui::XImageManager;
        using ::com::sun::star::graphic::XGraphic;

        String GetCommandText( const sal_Char* _pCommandURL, const ::rtl::OUString& _rModuleName )
        {
            ::rtl::OUString sLabel;
            if ( !_pCommandURL || !*_pCommandURL )
                return sLabel;

            Reference< XNameAccess > xUICommandLabels;
            ::rtl::OUString sCommandURL = ::rtl::OUString::createFromAscii( _pCommandURL );

            try
            {
                do
                {
                    // Retrieve popup menu labels
                    Reference< XMultiServiceFactory > xFactory( ::comphelper::getProcessServiceFactory() );
                    if ( !xFactory.is() )
                        break;

                    Reference< XNameAccess> xNameAccess;
                    xNameAccess = xNameAccess.query( xFactory->createInstance(
                        ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "com.sun.star.frame.UICommandDescription" ) )
                    ) );
                    if ( !xNameAccess.is() )
                        break;

                    xNameAccess->getByName( _rModuleName ) >>= xUICommandLabels;
                    if ( !xUICommandLabels.is() )
                        break;

                    Sequence< PropertyValue > aProperties;
                    if ( !( xUICommandLabels->getByName(sCommandURL) >>= aProperties ) )
                        break;

                    sal_Int32 nCount( aProperties.getLength() );
                    for ( sal_Int32 i=0; i<nCount; ++i )
                    {
                        ::rtl::OUString sPropertyName( aProperties[i].Name );
                        if ( sPropertyName.equalsAscii("Label" ) )
                        {
                            aProperties[i].Value >>= sLabel;
                            break;
                        }
                    }
                }
                while ( false );
            }
            catch( Exception& rException )
            {
                (void)rException;
            }

            return sLabel;
        }

        Image GetCommandIcon( const sal_Char* _pCommandURL, const ::rtl::OUString& _rModuleName )
        {
            Image aIcon;
            if ( !_pCommandURL || !*_pCommandURL )
                return aIcon;

            Reference< XNameAccess > xUICommandLabels;
            ::rtl::OUString sCommandURL = ::rtl::OUString::createFromAscii( _pCommandURL );
            try
            {
                do
                {
                    // Retrieve popup menu labels
                    Reference< XMultiServiceFactory> xFactory( ::comphelper::getProcessServiceFactory() );
                    if ( !xFactory.is() )
                        break;

                    Reference< XModuleUIConfigurationManagerSupplier > xSupplier(
                        xFactory->createInstance( ::rtl::OUString(RTL_CONSTASCII_USTRINGPARAM(
                            "com.sun.star.ui.ModuleUIConfigurationManagerSupplier" ) ) ),
                        UNO_QUERY_THROW );

                    Reference< XUIConfigurationManager > xManager( xSupplier->getUIConfigurationManager( _rModuleName ) );
                    Reference< XImageManager > xImageManager;
                    if ( xManager.is() )
                        xImageManager = xImageManager.query( xManager->getImageManager() );
                    if ( !xImageManager.is() )
                        break;

                    Sequence< ::rtl::OUString > aCommandList( &sCommandURL, 1 );
                    Sequence<Reference< XGraphic> > xIconList( xImageManager->getImages( 0, aCommandList ) );
                    if ( !xIconList.hasElements() )
                        break;

                    aIcon = Graphic( xIconList[0] ).GetBitmapEx();
                }
                while ( false );
            }
            catch ( Exception& rException )
            {
                (void)rException;
            }

            return aIcon;
        }


    }

	//====================================================================
	//= OpenButton
	//====================================================================
	//--------------------------------------------------------------------
    OpenDocumentButton::OpenDocumentButton( Window* _pParent, const sal_Char* _pAsciiModuleName, const ResId& _rResId )
        :PushButton( _pParent, _rResId )
    {
        impl_init( _pAsciiModuleName );
    }

    //--------------------------------------------------------------------
    void OpenDocumentButton::impl_init( const sal_Char* _pAsciiModuleName )
    {
        DBG_ASSERT( _pAsciiModuleName, "OpenDocumentButton::impl_init: invalid module name!" );
        m_sModule = ::rtl::OUString::createFromAscii( _pAsciiModuleName );

        // our label should equal the UI text of the "Open" command
        String sLabel( GetCommandText( ".uno:Open", m_sModule ) );
        sLabel.SearchAndReplaceAllAscii( "~", String() );
        sLabel.Insert( (sal_Unicode)' ', 0 );
        SetText( sLabel );

        // Place icon left of text and both centered in the button.
        SetModeImage( GetCommandIcon( ".uno:Open", m_sModule ), BMP_COLOR_NORMAL );
        EnableImageDisplay( sal_True );
        EnableTextDisplay( sal_True );
        SetImageAlign( IMAGEALIGN_LEFT );
        SetStyle( GetStyle() | WB_CENTER );
    }

	//====================================================================
	//= OpenDocumentListBox
	//====================================================================
    //--------------------------------------------------------------------
    OpenDocumentListBox::OpenDocumentListBox( Window* _pParent, const sal_Char* _pAsciiModuleName, const ResId& _rResId )
        :ListBox( _pParent, _rResId )
    {
        impl_init( _pAsciiModuleName );
    }

    //--------------------------------------------------------------------
    void OpenDocumentListBox::impl_init( const sal_Char* _pAsciiModuleName )
    {
        DBG_ASSERT( _pAsciiModuleName, "OpenDocumentListBox::impl_init: invalid module name!" );

        Sequence< Sequence< PropertyValue> > aHistory = SvtHistoryOptions().GetList( ePICKLIST );
        Reference< XNameAccess > xFilterFactory;
        xFilterFactory = xFilterFactory.query( ::comphelper::getProcessServiceFactory()->createInstance(
            ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "com.sun.star.document.FilterFactory" ) ) ) );

        sal_uInt32 nCount = aHistory.getLength();
        for ( sal_uInt32 nItem = 0; nItem < nCount; ++nItem )
        {
            try
            {
                //  Get the current history item's properties.
                ::comphelper::SequenceAsHashMap aItemProperties( aHistory[ nItem ] );
                ::rtl::OUString sURL = aItemProperties.getUnpackedValueOrDefault( HISTORY_PROPERTYNAME_URL, ::rtl::OUString() );
                ::rtl::OUString sFilter = aItemProperties.getUnpackedValueOrDefault( HISTORY_PROPERTYNAME_FILTER, ::rtl::OUString() );
                String          sTitle = aItemProperties.getUnpackedValueOrDefault( HISTORY_PROPERTYNAME_TITLE, ::rtl::OUString() );
                ::rtl::OUString sPassword = aItemProperties.getUnpackedValueOrDefault( HISTORY_PROPERTYNAME_PASSWORD, ::rtl::OUString() );

                //  If the entry is an impress file then insert it into the
                //  history list and the list box.
                Sequence< PropertyValue > aProps;
                xFilterFactory->getByName( sFilter ) >>= aProps;

                ::comphelper::SequenceAsHashMap aFilterProperties( aProps );
                ::rtl::OUString sDocumentService = aFilterProperties.getUnpackedValueOrDefault(
                    ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "DocumentService" ) ), ::rtl::OUString() );
                if ( sDocumentService.equalsAscii( _pAsciiModuleName ) )
                {
                    // yes, it's a Base document
                    INetURLObject aURL;
                    aURL.SetSmartURL( sURL );
                    // The password is set only when it is not empty.
                    if ( sPassword.getLength() > 0 )
                        aURL.SetPass( sPassword );

                    if ( !sTitle.Len() )
                        sTitle = aURL.getBase( INetURLObject::LAST_SEGMENT, true, INetURLObject::DECODE_UNAMBIGUOUS );

                    String sDecodedURL = aURL.GetMainURL( INetURLObject::NO_DECODE );

                    sal_uInt16 nPos = InsertEntry( sTitle );
                    m_aURLs.insert( MapIndexToStringPair::value_type( nPos, StringPair( sDecodedURL, sFilter ) ) );
                }
            }
            catch( Exception& rException )
            {
                (void)rException;
            }
        }
    }

    //--------------------------------------------------------------------
    String OpenDocumentListBox::GetSelectedDocumentURL() const
    {
        String sURL;
        sal_uInt16 nSelected = GetSelectEntryPos();
        if ( LISTBOX_ENTRY_NOTFOUND != GetSelectEntryPos() )
            sURL = impl_getDocumentAtIndex( nSelected ).first;
        return sURL;
    }

    //--------------------------------------------------------------------
    String OpenDocumentListBox::GetSelectedDocumentFilter() const
    {
        String sFilter;
        sal_uInt16 nSelected = GetSelectEntryPos();
        if ( LISTBOX_ENTRY_NOTFOUND != GetSelectEntryPos() )
            sFilter = impl_getDocumentAtIndex( nSelected ).second;
        return sFilter;
    }

    //--------------------------------------------------------------------
    OpenDocumentListBox::StringPair OpenDocumentListBox::impl_getDocumentAtIndex( sal_uInt16 _nListIndex, bool _bSystemNotation ) const
    {
        MapIndexToStringPair::const_iterator pos = m_aURLs.find( _nListIndex );
        DBG_ASSERT( pos != m_aURLs.end(), "OpenDocumentListBox::impl_getDocumentAtIndex: invalid index!" );

        StringPair aDocumentDescriptor;
        if ( pos != m_aURLs.end() )
        {
            aDocumentDescriptor = pos->second;
            if ( _bSystemNotation && aDocumentDescriptor.first.Len() )
            {
                ::svt::OFileNotation aNotation( aDocumentDescriptor.first );
                aDocumentDescriptor.first = aNotation.get( ::svt::OFileNotation::N_SYSTEM );
            }
        }
        return aDocumentDescriptor;
    }

    //--------------------------------------------------------------------
    void  OpenDocumentListBox::RequestHelp( const HelpEvent& _rHEvt )
    {
	    if( !( _rHEvt.GetMode() & HELPMODE_QUICK ) )
            return;
        if ( !IsEnabled() )
            return;

        Point aRequestPos( ScreenToOutputPixel( _rHEvt.GetMousePosPixel() ) );
        sal_uInt16 nItemIndex = LISTBOX_ENTRY_NOTFOUND;
        if ( GetIndexForPoint( aRequestPos, nItemIndex ) != -1 )
        {
            Rectangle aItemRect( GetBoundingRectangle( nItemIndex ) );
            aItemRect = Rectangle(
                OutputToScreenPixel( aItemRect.TopLeft() ),
                OutputToScreenPixel( aItemRect.BottomRight() ) );
            String sHelpText = impl_getDocumentAtIndex( nItemIndex, true ).first;
            Help::ShowQuickHelp( this, aItemRect, sHelpText, QUICKHELP_LEFT | QUICKHELP_VCENTER );
        }
    }

//........................................................................
} // namespace dbaui
//........................................................................
