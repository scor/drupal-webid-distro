<?php

/**
 * @copyright
 *     WebIDauth - WebID authentication module for Drupal
 *     Copyright (C) 2012  mEducator Best Practice Network
 * @author
 *      Lazaros Ioannidis
 *      Charalampos Bratsas
 *      Dimitris Spachos
 */

class FoafParser
{
    /**
     * FoafParser::__construct()
     * Constructs a new instance of the FoafParser class
     * @param mixed $uri
     * @return
     */
    public function __construct($uri)
    {
        $this->graphite = new Graphite();
                $this->graphite->ns("cert", "http://www.w3.org/ns/auth/cert#");
                $this->graphite->ns("rsa", "http://www.w3.org/ns/auth/rsa#");
        $this->graphite->load($uri);
        $this->uri = $uri;
    }

    static private $graphite;
    private $uri;

    /**
     * FoafParser::getName()
     * Returns the name of the person described in the foaf profile
     * @return
     */
    public function getName()
    {
        $value = (string)$this->graphite->resource($this->uri)->get("foaf:name");
        if ($value && $value != '[NULL]')
            return $value;
    }

    /**
     * FoafParser::getImage()
     * Returns the image url of the person described in the foaf profile
     * @return
     */
    public function getImage()
    {
        $value = (string)$this->graphite->resource($this->uri)->get("foaf:img");
        if ($value && $value != '[NULL]')
            return $value;
    }

    /**
     * FoafParser::getMailbox()
     * Returns the email address of the person described in the foaf profile
     * @return
     */
    public function getMailbox()
    {
        $prefix = 'mailto:';
        $mbox = (string )$this->graphite->resource($this->uri)->get("foaf:mbox");

        if ($mbox=='[NULL]') return '';

        if (substr($mbox, 0, strlen($prefix) ) == $prefix) {
            $mbox = substr($mbox, strlen($prefix), strlen($mbox) );
        }


        return $mbox ;
    }

}
